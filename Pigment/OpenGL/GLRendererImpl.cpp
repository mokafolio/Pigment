#include <Pigment/OpenGL/GLRendererImpl.hpp>
#include <Pigment/OpenGL/GLCommandBufferImpl.hpp>

#if STICK_PLATFORM == STICK_PLATFORM_OSX
#include <dlfcn.h>
#endif

#define ASSERT_NO_GL_ERROR(_func) do { _func; \
GLenum err = glGetError(); \
if(err != GL_NO_ERROR) \
{ \
switch(err) \
{ \
case GL_NO_ERROR: \
printf("%s line %i GL_NO_ERROR: No error has been recorded.\n", __FILE__, __LINE__);\
break; \
case GL_INVALID_ENUM: \
printf("%s line %i GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.\n", __FILE__, __LINE__);\
break; \
case GL_INVALID_VALUE: \
printf("%s line %i GL_INVALID_VALUE: A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.\n", __FILE__, __LINE__);\
break; \
case GL_INVALID_OPERATION: \
printf("%s line %i GL_INVALID_OPERATION: The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.\n", __FILE__, __LINE__);\
break; \
case GL_INVALID_FRAMEBUFFER_OPERATION: \
printf("%s line %i GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.\n", __FILE__, __LINE__);\
break; \
case GL_OUT_OF_MEMORY: \
printf("%s line %i GL_OUT_OF_MEMORY: There is not enough memory left to executeLua the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.\n", __FILE__, __LINE__);\
break; \
} \
exit(EXIT_FAILURE); \
} \
} while(false)

#if STICK_PLATFORM == STICK_PLATFORM_LINUX
#undef None //??? some glx thing
#endif //STICK_PLATFORM == STICK_PLATFORM_LINUX

namespace pigment
{
    namespace detail
    {
        struct GLTextureFormat
        {
            GLenum glFormat;
            GLenum glInternalFormat;
        };

        static GLTextureFormat s_glTextureFormats[] =
        {
            //R8
            {GL_RED, GL_R8, },
            //R16
            {GL_RED, GL_R16},
            //R32
            {GL_RED, GL_R32UI},
            //R16F
            {GL_RED, GL_R16F},
            //R32F
            {GL_RED, GL_R32F},
            //RGB8
            {GL_RGB, GL_RGB8},
            //RGB16
            {GL_RGB, GL_RGB16},
            //RGB32
            {GL_RGB, GL_RGB32UI},
            //RGB16F
            {GL_RGB, GL_RGB16F},
            //RGB32F
            {GL_RGB, GL_RGB32F},
            //BGR8
            {GL_BGR, GL_RGB8},
            //BGR16
            {GL_BGR, GL_RGB16},
            //BGR32
            {GL_BGR, GL_RGB32UI},
            //BGR16F
            {GL_BGR, GL_RGB16F},
            //BGR32F
            {GL_BGR, GL_RGB32F},
            //RGBA8
            {GL_RGBA, GL_RGBA8},
            //RGBA16
            {GL_RGBA, GL_RGBA16},
            //RGBA32
            {GL_RGBA, GL_RGBA32UI},
            //RGBA16F
            {GL_RGBA, GL_RGBA16F},
            //RGBA32F
            {GL_RGBA, GL_RGBA32F},
            //BGRA8
            {GL_BGRA, GL_RGBA8},
            //BGRA16
            {GL_BGRA, GL_RGBA16},
            //BGRA32
            {GL_BGRA, GL_RGBA32UI},
            //BGRA16F
            {GL_BGRA, GL_RGBA16F},
            //BGRA32F
            {GL_BGRA, GL_RGBA32F},
            //Depth16
            {GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT16},
            //Depth24
            {GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT24},
            //Depth32
            {GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT32},
            //Depth24Stencil8
            {GL_DEPTH_STENCIL, GL_DEPTH24_STENCIL8},
            //Depth32F
            {GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT32F},
            //Depth32FStencil8
            {GL_DEPTH_STENCIL, GL_DEPTH32F_STENCIL8}
        };

        static_assert((Size)TextureFormat::Count == sizeof(s_glTextureFormats) / sizeof(s_glTextureFormats[0]), "TextureFormat mapping is not complete!");

        static GLenum s_glReadFormats[] =
        {
            //Red
            GL_RED,
            //Green
            GL_GREEN,
            //Blue
            GL_BLUE,
            //Alpha
            GL_ALPHA,
            //RGB
            GL_RGB,
            //RGBA
            GL_RGBA,
            //BGR
            GL_BGR,
            //BGRA
            GL_BGRA
        };

        static_assert((Size)ReadFormat::Count == sizeof(s_glReadFormats) / sizeof(s_glReadFormats[0]), "ReadFormat mapping is not complete!");

        static GLenum s_glDataTypes[] =
        {
            //UInt8
            GL_UNSIGNED_BYTE,
            //Int8
            GL_BYTE,
            //UInt16
            GL_UNSIGNED_SHORT,
            //Int16
            GL_SHORT,
            //UInt32
            GL_UNSIGNED_INT,
            //Int32
            GL_INT,
            //Float32
            GL_FLOAT,
            //Float64
            GL_DOUBLE
        };

        static_assert((Size)BufferDataType::Count == sizeof(s_glDataTypes) / sizeof(s_glDataTypes[0]), "BufferDataType mapping is not complete!");

        static GLenum s_glBufferUsage[] =
        {
            //DynamicDraw
            GL_DYNAMIC_DRAW,
            //StaticDraw
            GL_STATIC_DRAW,
            //StreamDraw
            GL_STREAM_DRAW,
            //DynamicRead
            GL_DYNAMIC_READ,
            //StaticRead
            GL_STATIC_READ,
            //StreamRead
            GL_STREAM_READ,
            //DynamicCopy
            GL_DYNAMIC_COPY,
            //StaticCopy
            GL_STATIC_COPY,
            //StreamCopy
            GL_STREAM_COPY
        };

        static_assert((Size)BufferUsage::Count == sizeof(s_glBufferUsage) / sizeof(s_glBufferUsage[0]), "BufferUsage mapping is not complete!");

        static GLenum s_glCompareFuncs[] =
        {
            //Equal
            GL_EQUAL,
            //LessEqual
            GL_LEQUAL,
            //GreaterEqual
            GL_GEQUAL,
            //NotEqual
            GL_NOTEQUAL,
            //Always
            GL_ALWAYS,
            //Never
            GL_NEVER,
            //Less
            GL_LESS,
            //Greater
            GL_GREATER
        };

        static_assert((Size)CompareFunction::Count == sizeof(s_glCompareFuncs) / sizeof(s_glCompareFuncs[0]), "CompareFunction mapping is not complete!");

        static GLenum s_glStencilOps[] =
        {
            //Keep
            GL_KEEP,
            //Zero
            GL_ZERO,
            //Replace
            GL_REPLACE,
            //Increment
            GL_INCR,
            //IncrementWrap
            GL_INCR_WRAP,
            //Decrement
            GL_DECR,
            //DecrementWrap
            GL_DECR_WRAP,
            //Invert
            GL_INVERT
        };

        static GLenum s_glBlendModes[] =
        {
            //Add
            GL_FUNC_ADD,
            //Substract
            GL_FUNC_SUBTRACT,
            //ReverseSubstract
            GL_FUNC_REVERSE_SUBTRACT,
            //Min
            GL_MIN,
            //Max
            GL_MAX
        };

        static_assert((Size)BlendMode::Count == sizeof(s_glBlendModes) / sizeof(s_glBlendModes[0]), "BlendMode mapping is not complete!");

        static GLenum s_glBlendFuncs[] =
        {
            //Zero
            GL_ZERO,
            //One
            GL_ONE,
            //SourceColor
            GL_SRC_COLOR,
            //InverseSourceColor
            GL_ONE_MINUS_SRC_COLOR,
            //DestinationColor
            GL_DST_COLOR,
            //InverseDestinationColor
            GL_ONE_MINUS_DST_COLOR,
            //SourceAlpha
            GL_SRC_ALPHA,
            //InverseSourceAlpha
            GL_ONE_MINUS_SRC_ALPHA,
            //DestinationAlpha
            GL_DST_ALPHA,
            //InverseDestinationAlpha
            GL_ONE_MINUS_DST_ALPHA,
            //ConstantColor
            GL_CONSTANT_COLOR,
            //InverseConstantColor
            GL_ONE_MINUS_CONSTANT_COLOR,
            //ConstantAlpha
            GL_CONSTANT_ALPHA,
            //InverseConstantAlpha
            GL_ONE_MINUS_CONSTANT_ALPHA
        };

        static_assert((Size)BlendFunction::Count == sizeof(s_glBlendFuncs) / sizeof(s_glBlendFuncs[0]), "BlendFunction mapping is not complete!");

        static GLenum s_glFaceType[] =
        {
            //None
            GL_NONE, //this should never happen
            //Front
            GL_FRONT,
            //Back
            GL_BACK,
            //FrontAndBack
            GL_FRONT_AND_BACK
        };

        static_assert((Size)FaceType::Count == sizeof(s_glFaceType) / sizeof(s_glFaceType[0]), "FaceType mapping is not complete!");

        static GLenum s_glVertexDrawModes [] =
        {
            //Triangles
            GL_TRIANGLES,
            //TrianglesAdjacency
            GL_TRIANGLES_ADJACENCY,
            //TriangleStrip
            GL_TRIANGLE_STRIP,
            //TriangleStripAdjacency
            GL_TRIANGLE_STRIP_ADJACENCY,
            //TriangleFan
            GL_TRIANGLE_FAN,
            //Points
            GL_POINTS,
            //Lines
            GL_LINES,
            //LinesAdjacency
            GL_LINES_ADJACENCY,
            //LineStrip
            GL_LINE_STRIP,
            //LineStripAdjacency
            GL_LINE_STRIP_ADJACENCY,
            //LineLoop
            GL_LINE_LOOP
        };

        static_assert((Size)VertexDrawMode::Count == sizeof(s_glVertexDrawModes) / sizeof(s_glVertexDrawModes[0]), "VertexDrawMode mapping is not complete!");

        static GLenum s_glWrap [] =
        {
            //Repeat
            GL_REPEAT,
            //ClampToEdge
            GL_CLAMP_TO_EDGE,
            //ClampToBorder
            GL_CLAMP_TO_BORDER
        };

        static_assert((Size)TextureWrap::Count == sizeof(s_glWrap) / sizeof(s_glWrap[0]), "TextureWrap mapping is not complete!");

        static GLenum s_glPixelDataType [] =
        {
            //UInt8
            GL_UNSIGNED_BYTE,
            //Int8
            GL_BYTE,
            //UInt16
            GL_UNSIGNED_SHORT,
            //Int16
            GL_SHORT,
            //UInt32
            GL_UNSIGNED_INT,
            //Int32
            GL_INT,
            //Float32
            GL_FLOAT
        };

        static_assert((Size)PixelDataType::Count == sizeof(s_glPixelDataType) / sizeof(s_glPixelDataType[0]), "PixelDataType mapping is not complete!");


        void * getGLFunctionPointer(const char * _name)
        {
#if STICK_PLATFORM == STICK_PLATFORM_OSX
            return (void *)dlsym(RTLD_DEFAULT, _name);
#elif STICK_PLATFORM == STICK_PLATFORM_LINUX
            return (void *)glXGetProcAddress((GLubyte *)_name);
#endif
        }

        RendererImpl::RendererImpl(Allocator & _alloc) :
            m_textures(this, _alloc),
            m_samplers(this, _alloc),
            m_programs(this, _alloc),
            m_meshes(this, _alloc),
            m_vertexBuffers(this, _alloc),
            m_indexBuffers(this, _alloc),
            m_renderStates(this, _alloc),
            m_renderBuffers(this, _alloc),
            m_commandBuffers(_alloc),
            m_commandBufferQueue(_alloc),
            m_alloc(&_alloc)
        {
            m_glProgramUniform1i = (PFNGLPROGRAMUNIFORM1IPROC)getGLFunctionPointer("glProgramUniform1i");
            m_glProgramUniform1f = (PFNGLPROGRAMUNIFORM1FPROC)getGLFunctionPointer("glProgramUniform1f");
            m_glProgramUniform2fv = (PFNGLPROGRAMUNIFORM2FVPROC)getGLFunctionPointer("glProgramUniform2fv");
            m_glProgramUniform3fv = (PFNGLPROGRAMUNIFORM3FVPROC)getGLFunctionPointer("glProgramUniform3fv");
            m_glProgramUniform4fv = (PFNGLPROGRAMUNIFORM4FVPROC)getGLFunctionPointer("glProgramUniform4fv");
            m_glProgramUniformMatrix3fv = (PFNGLPROGRAMUNIFORMMATRIX3FVPROC)getGLFunctionPointer("glProgramUniformMatrix3fv");
            m_glProgramUniformMatrix4fv = (PFNGLPROGRAMUNIFORMMATRIX4FVPROC)getGLFunctionPointer("glProgramUniformMatrix4fv");
            STICK_ASSERT(m_glProgramUniform1i);
            STICK_ASSERT(m_glProgramUniform1f);
            STICK_ASSERT(m_glProgramUniform2fv);
            STICK_ASSERT(m_glProgramUniform3fv);
            STICK_ASSERT(m_glProgramUniform4fv);
            STICK_ASSERT(m_glProgramUniformMatrix3fv);
            STICK_ASSERT(m_glProgramUniformMatrix4fv);
        }

        RendererImpl::~RendererImpl()
        {

        }

        CommandBufferResult RendererImpl::createCommandBuffer()
        {
            auto mem = m_alloc->allocate(sizeof(CommandBuffer));
            m_commandBuffers.append(UniquePtr<CommandBuffer>(new(mem.ptr)CommandBuffer(*m_alloc)));
            return *m_commandBuffers.back();
        }

        void RendererImpl::destroyCommandBuffer(CommandBuffer & _buffer)
        {
            auto it = findIf(m_commandBuffers.begin(), m_commandBuffers.end(), [&](const UniquePtr<CommandBuffer> & _buf) { return _buf.get() == &_buffer; });
            if (it != m_commandBuffers.end())
            {
                m_commandBuffers.remove(it);
            }
        }

        TextureHandleResult RendererImpl::createTexture()
        {
            GLuint handle;
            ASSERT_NO_GL_ERROR(glGenTextures(1, &handle));
            return m_textures.append({handle, 0, 0, 0, GL_TEXTURE_2D, TextureFormat::RGBA8, 0, RenderBufferHandle()});
        }

        void RendererImpl::destroyTexture(const TextureHandle & _tex)
        {
            auto tex = m_textures.get(_tex);
            ASSERT_NO_GL_ERROR(glDeleteTextures(1, &tex.glHandle));
            m_textures.remove(_tex);
        }

        SamplerHandleResult RendererImpl::createSampler(const SamplerSettings & _settings)
        {
            GLuint glHandle;
            GLenum minFilter, magFilter;
            minFilter = magFilter = GL_NEAREST;
            bool bMipMapping = _settings.useMipMapping;
            if (_settings.filtering == TextureFiltering::Nearest)
            {
                if (bMipMapping)
                    minFilter = GL_NEAREST_MIPMAP_NEAREST;
                else
                    minFilter = GL_NEAREST;

                magFilter = GL_NEAREST;
            }
            else if (_settings.filtering == TextureFiltering::Bilinear)
            {
                if (bMipMapping)
                    minFilter = GL_LINEAR_MIPMAP_NEAREST;
                else
                    minFilter = GL_LINEAR;

                magFilter = GL_LINEAR;
            }
            else if (_settings.filtering == TextureFiltering::Trilinear)
            {
                if (bMipMapping)
                    minFilter = GL_LINEAR_MIPMAP_LINEAR;
                else
                    minFilter = GL_LINEAR;

                magFilter = GL_LINEAR;
            }

            ASSERT_NO_GL_ERROR(glGenSamplers(1, &glHandle));
            ASSERT_NO_GL_ERROR(glSamplerParameteri(glHandle, GL_TEXTURE_WRAP_S, s_glWrap[static_cast<Size>(_settings.wrapS)]));
            ASSERT_NO_GL_ERROR(glSamplerParameteri(glHandle, GL_TEXTURE_WRAP_T, s_glWrap[static_cast<Size>(_settings.wrapT)]));
            ASSERT_NO_GL_ERROR(glSamplerParameteri(glHandle, GL_TEXTURE_WRAP_T, s_glWrap[static_cast<Size>(_settings.wrapR)]));
            ASSERT_NO_GL_ERROR(glSamplerParameteri(glHandle, GL_TEXTURE_MAG_FILTER, magFilter));
            ASSERT_NO_GL_ERROR(glSamplerParameteri(glHandle, GL_TEXTURE_MIN_FILTER, minFilter));

            return m_samplers.append({glHandle});
        }

        void RendererImpl::destroySampler(const SamplerHandle & _sampler)
        {
            auto s = m_samplers.get(_sampler);
            ASSERT_NO_GL_ERROR(glDeleteSamplers(1, &s.glHandle));
            m_samplers.remove(_sampler);
        }

        static Error validateFrameBuffer()
        {
            GLenum err;
            err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            switch (err)
            {
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                return Error(ec::BadRenderBuffer, "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: Invalid OpenGL FBO attachment!", STICK_FILE, STICK_LINE);
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                return Error(ec::BadRenderBuffer, "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: OpenGL FBO has no attachments!", STICK_FILE, STICK_LINE);
            case GL_FRAMEBUFFER_UNSUPPORTED:
                return Error(ec::BadRenderBuffer, "GL_FRAMEBUFFER_UNSUPPORTED: The OpenGL format combination is not supported by FBOs on your platform!", STICK_FILE, STICK_LINE);
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                return Error(ec::BadRenderBuffer, "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER!", STICK_FILE, STICK_LINE);
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                return Error(ec::BadRenderBuffer, "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER!", STICK_FILE, STICK_LINE);
            }

            return Error();
        }

        RenderBufferResult RendererImpl::createRenderBuffer(const RenderBufferSettings & _settings)
        {
            RenderBufferHandle rbh = m_renderBuffers.append(GLRenderBuffer());
            GLRenderBuffer & renderBuffer = m_renderBuffers.get(rbh);
            renderBuffer.sampleCount = _settings.sampleCount;
            renderBuffer.width = _settings.width;
            renderBuffer.height = _settings.height;
            renderBuffer.bDirty = false;
            ASSERT_NO_GL_ERROR(glGenFramebuffers(1, &renderBuffer.glHandle));
            if (_settings.sampleCount > 1)
            {
                ASSERT_NO_GL_ERROR(glGenFramebuffers(1, &renderBuffer.glHandleMSAA));
            }

            GLuint nextColorTargetID = 0;
            for (const auto & rt : _settings.renderTargets)
            {
                const TextureFormatInfo & info = s_textureFormatInfos[static_cast<Size>(rt.format)];
                const GLTextureFormat & format = s_glTextureFormats[static_cast<Size>(rt.format)];
                bool bIsColorAttachment = info.bIsColorFormat;

                GLuint texHandle;
                ASSERT_NO_GL_ERROR(glGenTextures(1, &texHandle));

                ASSERT_NO_GL_ERROR(glActiveTexture(GL_TEXTURE0));
                ASSERT_NO_GL_ERROR(glBindTexture(GL_TEXTURE_2D, texHandle));

                ASSERT_NO_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0));
                ASSERT_NO_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, rt.mipmapLevelCount));

                GLenum dt = format.glFormat == GL_DEPTH_STENCIL ? GL_UNSIGNED_INT_24_8 : GL_UNSIGNED_BYTE;

                ASSERT_NO_GL_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, format.glInternalFormat, (GLuint)_settings.width, (GLuint)_settings.height, 0, format.glFormat, dt, 0));

                TextureHandle th = m_textures.append({texHandle, _settings.width, _settings.height, 0, GL_TEXTURE_2D, rt.format, 0, rbh});

                ASSERT_NO_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, renderBuffer.glHandle));

                GLRenderBuffer::RenderTarget target = {0, 0};
                if (bIsColorAttachment)
                {
                    ASSERT_NO_GL_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + nextColorTargetID,
                                       GL_TEXTURE_2D, texHandle, 0));
                    renderBuffer.colorAttachmentPoints.append(GL_COLOR_ATTACHMENT0 + nextColorTargetID);
                    target.attachmentPoint = GL_COLOR_ATTACHMENT0 + nextColorTargetID;
                    renderBuffer.colorTargets.append(th);
                }
                else
                {
                    //if we don't set this, the framebuffer validation fails if we only have a depth tex attached
                    ASSERT_NO_GL_ERROR(glDrawBuffer(GL_NONE));
                    ASSERT_NO_GL_ERROR(glReadBuffer(GL_NONE));
                    ASSERT_NO_GL_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, !info.bIsStencilFormat ? GL_DEPTH_ATTACHMENT : GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texHandle, 0));

                    target.attachmentPoint = info.bIsStencilFormat ? GL_DEPTH_ATTACHMENT : GL_DEPTH_STENCIL_ATTACHMENT;
                    renderBuffer.depthTarget = th;
                }

                Error err = validateFrameBuffer();
                if (err)
                {
                    destroyRenderBuffer(rbh, true);
                    return err;
                }

                if (_settings.sampleCount > 1)
                {
                    Int32 maxSamples;

                    ASSERT_NO_GL_ERROR(glGetIntegerv(GL_MAX_SAMPLES, &maxSamples));

                    if (renderBuffer.sampleCount > maxSamples)
                        renderBuffer.sampleCount = maxSamples;

                    ASSERT_NO_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, renderBuffer.glHandleMSAA));

                    GLuint glrb;
                    ASSERT_NO_GL_ERROR(glGenRenderbuffers(1, &glrb));
                    ASSERT_NO_GL_ERROR(glBindRenderbuffer(GL_RENDERBUFFER, glrb));
                    ASSERT_NO_GL_ERROR(glRenderbufferStorageMultisample(GL_RENDERBUFFER, renderBuffer.sampleCount, format.glInternalFormat, (GLuint)renderBuffer.width, (GLuint)renderBuffer.height));

                    if (bIsColorAttachment)
                    {
                        ASSERT_NO_GL_ERROR(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + nextColorTargetID, GL_RENDERBUFFER, glrb));
                    }
                    else
                    {
                        ASSERT_NO_GL_ERROR(glDrawBuffer(GL_NONE));
                        ASSERT_NO_GL_ERROR(glReadBuffer(GL_NONE));
                        ASSERT_NO_GL_ERROR(glFramebufferRenderbuffer(GL_FRAMEBUFFER, !info.bIsStencilFormat ? GL_DEPTH_ATTACHMENT : GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, glrb));
                    }
                    target.msaaRenderBuffer = glrb;

                    renderBuffer.attachmentMap[th.m_index] = target;

                    err = validateFrameBuffer();
                    if (err)
                    {
                        destroyRenderBuffer(rbh, true);
                        return err;
                    }

                    if (bIsColorAttachment)
                        nextColorTargetID++;;
                }
            }

            ASSERT_NO_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));

            return RenderBuffer(rbh, renderBuffer.colorTargets, renderBuffer.depthTarget);
        }

        void RendererImpl::destroyRenderBuffer(const RenderBufferHandle & _renderBufferHandle, bool _bDestroyRenderTargets)
        {
            GLRenderBuffer & rb = m_renderBuffers.get(_renderBufferHandle);
            ASSERT_NO_GL_ERROR(glDeleteFramebuffers(1, &rb.glHandle));
            if (rb.glHandleMSAA)
            {
                auto rit = rb.attachmentMap.begin();
                for (; rit != rb.attachmentMap.end(); ++rit)
                {
                    ASSERT_NO_GL_ERROR(glDeleteRenderbuffers(1,  &(rit->value.msaaRenderBuffer)));
                }
                ASSERT_NO_GL_ERROR(glDeleteFramebuffers(1, &rb.glHandleMSAA));
            }

            if (_bDestroyRenderTargets)
            {
                for (auto & ct : rb.colorTargets)
                {
                    destroyTexture(ct);
                }
            }
            else
            {
                for (auto & ct : rb.colorTargets)
                {
                    m_textures.get(ct).renderBuffer = RenderBufferHandle();
                }
            }
            m_renderBuffers.remove(_renderBufferHandle);
        }

        static Error compileShader(const String & _shaderCode, GLenum _shaderType, GLuint & _outHandle)
        {
            Error ret;
            GLenum glHandle = glCreateShader(_shaderType);
            const char * cstr = _shaderCode.cString();
            GLint len = (GLint)_shaderCode.length();
            ASSERT_NO_GL_ERROR(glShaderSource(glHandle, 1, &cstr, &len));
            ASSERT_NO_GL_ERROR(glCompileShader(glHandle));

            //check if the shader compiled
            GLint state;
            ASSERT_NO_GL_ERROR(glGetShaderiv(glHandle, GL_COMPILE_STATUS, &state));
            if (state == GL_FALSE)
            {
                GLint infologLength;
                ASSERT_NO_GL_ERROR(glGetShaderiv(glHandle, GL_INFO_LOG_LENGTH, &infologLength));

                char * str = (char *)malloc(infologLength);
                ASSERT_NO_GL_ERROR(glGetShaderInfoLog(glHandle, infologLength, &infologLength, str));

                ret = Error(ec::BadGPUProgram, String::concat("Could not compile GLSL shader: ", str), STICK_FILE, STICK_LINE);
                glDeleteShader(glHandle);
                free(str);
            }
            else
            {
                _outHandle = glHandle;
            }
            return ret;
        }

        ProgramHandleResult RendererImpl::createProgram(const ProgramSettings & _settings)
        {
            GLuint vertexShader, geometryShader, fragmentShader;
            Error err = compileShader(_settings.vertexShaderCode, GL_VERTEX_SHADER, vertexShader);
            if (!err)
            {
                if (_settings.geometryShaderCode.length())
                {
                    err = compileShader(_settings.geometryShaderCode, GL_GEOMETRY_SHADER, geometryShader);
                }
                if (!err)
                {
                    err = compileShader(_settings.fragmentShaderCode, GL_FRAGMENT_SHADER, fragmentShader);
                }
            }
            if (err) return err;

            GLuint program = glCreateProgram();

            ASSERT_NO_GL_ERROR(glAttachShader(program, vertexShader));

            if (_settings.geometryShaderCode.length())
            {
                ASSERT_NO_GL_ERROR(glAttachShader(program, geometryShader));
            }

            ASSERT_NO_GL_ERROR(glAttachShader(program, fragmentShader));

            //bind the attribute locations from the layout
            const BufferLayout::BufferElementArray & elements = _settings.vertexLayout.elements();
            auto sit = elements.begin();

            for (; sit != elements.end(); ++sit)
            {
                ASSERT_NO_GL_ERROR(glBindAttribLocation(program, (*sit).m_location, (*sit).m_name.cString()));
            }

            ASSERT_NO_GL_ERROR(glLinkProgram(program));

            //check if we had success
            GLint state;
            ASSERT_NO_GL_ERROR(glGetProgramiv(program, GL_LINK_STATUS, &state));

            if (state == GL_FALSE)
            {
                GLint infologLength;
                ASSERT_NO_GL_ERROR(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infologLength));

                char * str = (char *)malloc(infologLength);
                ASSERT_NO_GL_ERROR(glGetProgramInfoLog(program, infologLength, &infologLength, str));

                err = Error(ec::BadGPUProgram, String::concat("Error linking GLSL program: ", str), STICK_FILE, STICK_LINE);
                free(str);
            }

            ASSERT_NO_GL_ERROR(glDeleteShader(vertexShader));
            if (_settings.geometryShaderCode.length())
            {
                ASSERT_NO_GL_ERROR(glDeleteShader(geometryShader));
            }
            ASSERT_NO_GL_ERROR(glDeleteShader(fragmentShader));

            if (err)
            {
                glDeleteProgram(program);
                return err;
            }

            GLProgram prog;
            prog.glHandle = program;
            //check what uniforms are active
            GLint uniformCount;
            ASSERT_NO_GL_ERROR(glUseProgram(program));
            ASSERT_NO_GL_ERROR(glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniformCount));
            for (GLint i = 0; i < uniformCount; ++i)
            {
                char nameBuffer[512] = {0};
                GLsizei len, size;
                GLenum type;
                ASSERT_NO_GL_ERROR(glGetActiveUniform(program, i, 512, &len, &size, &type, nameBuffer));
                GLuint loc = glGetUniformLocation(program, nameBuffer);
                ProgramVariableType mt = ProgramVariableType::None;

                //determine the type
                if (type == GL_FLOAT)
                {
                    mt = ProgramVariableType::Float32;
                }
                else if (type == GL_FLOAT_VEC2)
                {
                    mt = ProgramVariableType::Vec2f;
                }
                else if (type == GL_FLOAT_VEC3)
                {
                    mt = ProgramVariableType::Vec3f;
                }
                else if (type == GL_FLOAT_VEC4)
                {
                    mt = ProgramVariableType::Vec4f;
                }
                else if (type == GL_FLOAT_MAT3)
                {
                    mt = ProgramVariableType::Matrix3f;
                }
                else if (type == GL_FLOAT_MAT4)
                {
                    mt = ProgramVariableType::Matrix4f;
                }

                prog.uniforms.append({loc, mt, String(nameBuffer)});
                prog.uniformMap.insert(String(nameBuffer), prog.uniforms.count() - 1);
            }
            return m_programs.append(move(prog));
        }

        void RendererImpl::destroyProgram(const ProgramHandle & _handle)
        {
            auto p = m_programs.get(_handle);
            ASSERT_NO_GL_ERROR(glDeleteProgram(p.glHandle));
            m_programs.remove(_handle);
        }

        MeshHandleResult RendererImpl::createMesh(const MeshSettings & _settings)
        {
            GLuint glHandle;
            ASSERT_NO_GL_ERROR(glGenVertexArrays(1, &glHandle));
            ASSERT_NO_GL_ERROR(glBindVertexArray(glHandle));

            GLMesh mesh;
            mesh.glHandle = glHandle;
            Error err;
            for (const VertexBufferHandle & vbh : _settings.vertexBuffers)
            {
                const GLVertexBuffer & vb = m_vertexBuffers.get(vbh);
                ASSERT_NO_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, vb.glHandle));
                for (const auto & el : vb.layout.elements())
                {
                    if (el.m_elementCount <= 4)
                    {
                        ASSERT_NO_GL_ERROR(glVertexAttribPointer(el.m_location, (UInt32)el.m_elementCount, s_glDataTypes[static_cast<Size>(el.m_type)], GL_FALSE, (UInt32)el.m_stride, ((char *)0 + el.m_offset)));
                        ASSERT_NO_GL_ERROR(glEnableVertexAttribArray(el.m_location));

                        //check if this attribute is per instance rather than vertex
                        if (el.m_numberOfInstances > 0)
                        {
                            ASSERT_NO_GL_ERROR(glVertexAttribDivisor((UInt32)el.m_location, (UInt32)el.m_numberOfInstances));
                        }
                    }
                    //4X4 Matrix
                    else if (el.m_elementCount == 16 && el.m_numberOfInstances > 0)
                    {
                        ASSERT_NO_GL_ERROR(glEnableVertexAttribArray(el.m_location + 0));
                        ASSERT_NO_GL_ERROR(glEnableVertexAttribArray(el.m_location + 1));
                        ASSERT_NO_GL_ERROR(glEnableVertexAttribArray(el.m_location + 2));
                        ASSERT_NO_GL_ERROR(glEnableVertexAttribArray(el.m_location + 3));

                        GLenum dataType = s_glDataTypes[static_cast<Size>(el.m_type)];

                        ASSERT_NO_GL_ERROR(glVertexAttribPointer(el.m_location, 4, dataType,
                                           GL_FALSE, (UInt32)el.m_stride,
                                           ((char *)0 + el.m_offset)));
                        ASSERT_NO_GL_ERROR(glVertexAttribPointer(el.m_location + 1, 4, dataType,
                                           GL_FALSE, (UInt32)el.m_stride,
                                           ((char *)0 + el.m_offset + s_bufferDataTypeInfos[static_cast<Size>(el.m_type)].byteCount * 4)));
                        ASSERT_NO_GL_ERROR(glVertexAttribPointer(el.m_location + 2, 4, dataType,
                                           GL_FALSE, (UInt32)el.m_stride,
                                           ((char *)0 + el.m_offset + s_bufferDataTypeInfos[static_cast<Size>(el.m_type)].byteCount * 8)));
                        ASSERT_NO_GL_ERROR(glVertexAttribPointer(el.m_location + 3, 4, dataType,
                                           GL_FALSE, (UInt32)el.m_stride,
                                           ((char *)0 + el.m_offset + s_bufferDataTypeInfos[static_cast<Size>(el.m_type)].byteCount * 12)));


                        ASSERT_NO_GL_ERROR(glVertexAttribDivisor(el.m_location + 0, 1));
                        ASSERT_NO_GL_ERROR(glVertexAttribDivisor(el.m_location + 1, 1));
                        ASSERT_NO_GL_ERROR(glVertexAttribDivisor(el.m_location + 2, 1));
                        ASSERT_NO_GL_ERROR(glVertexAttribDivisor(el.m_location + 3, 1));
                    }
                    else
                    {
                        //Unsupported
                        err = Error(ec::UnsupportedRenderFeature, String::concat("The vertex element named ", el.m_name, " has an unsupported element count or configuration"), STICK_FILE, STICK_LINE);
                        break;
                    }
                }
                if (err) break;
                mesh.vertexBuffers.append(vbh);
            }
            if (err)
            {
                ASSERT_NO_GL_ERROR(glDeleteVertexArrays(1, &glHandle));
                return err;
            }
            if (_settings.indexBuffer.isValid())
            {
                const GLIndexBuffer & ib = m_indexBuffers.get(_settings.indexBuffer);
                ASSERT_NO_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib.glHandle));
            }

            ASSERT_NO_GL_ERROR(glBindVertexArray(0));
            return m_meshes.append(move(mesh));
        }

        void RendererImpl::destroyMesh(const MeshHandle & _mesh)
        {
            auto m = m_meshes.get(_mesh);
            glDeleteVertexArrays(1, &m.glHandle);
            m_meshes.remove(_mesh);
        }

        VertexBufferHandleResult RendererImpl::createVertexBuffer(const VertexLayout & _layout, BufferUsage _usage)
        {
            GLuint glHandle;
            ASSERT_NO_GL_ERROR(glGenBuffers(1, &glHandle));
            return m_vertexBuffers.append({glHandle, _layout, _usage});
        }

        void RendererImpl::destroyVertexBuffer(const VertexBufferHandle & _handle)
        {
            auto v = m_vertexBuffers.get(_handle);
            ASSERT_NO_GL_ERROR(glDeleteBuffers(1, &v.glHandle));
            m_vertexBuffers.remove(_handle);
        }

        IndexBufferHandleResult RendererImpl::createIndexBuffer(BufferUsage _usage)
        {
            GLuint glHandle;
            ASSERT_NO_GL_ERROR(glGenBuffers(1, &glHandle));
            return m_indexBuffers.append({glHandle, _usage});
        }

        void RendererImpl::destroyIndexBuffer(const IndexBufferHandle & _handle)
        {
            auto i = m_indexBuffers.get(_handle);
            ASSERT_NO_GL_ERROR(glDeleteBuffers(1, &i.glHandle));
            m_indexBuffers.remove(_handle);
        }

        RenderStateHandleResult RendererImpl::createRenderState(const RenderStateSettings & _settings)
        {
            return m_renderStates.append({_settings});
        }

        void RendererImpl::destoyRenderState(const RenderStateHandle & _handle)
        {
            m_renderStates.remove(_handle);
        }

        void RendererImpl::addCommandBuffer(const CommandBuffer & _buffer)
        {
            m_commandBufferQueue.append(_buffer.m_pimpl.get());
        }

        Error RendererImpl::submit()
        {
            Error ret;

            ProgramHandle lastProgram;
            RenderStateHandle lastRenderState;
            MeshHandle lastMesh;

            for (const CommandBufferImpl * buf : m_commandBufferQueue)
            {
                for (const auto & cmd : buf->m_queue)
                {
                    if (cmd.type == CommandBufferImpl::CommandType::LoadPixels)
                    {
                        GLenum type = GL_TEXTURE_1D;
                        if (cmd.command.loadPixelsCommand.height > 1)
                        {
                            if (cmd.command.loadPixelsCommand.depth > 1)
                            {
                                type = GL_TEXTURE_3D;
                            }
                            else
                            {
                                type = GL_TEXTURE_2D;
                            }
                        }

                        GLTexture & tex = m_textures.get(cmd.command.loadPixelsCommand.handle);
                        tex.mipmapLevelCount = cmd.command.loadPixelsCommand.mipmapLevelCount;
                        tex.glType = type;
                        GLenum glDataType = s_glPixelDataType[static_cast<Size>(cmd.command.loadPixelsCommand.dataType)];
                        const GLTextureFormat & format = s_glTextureFormats[static_cast<Size>(cmd.command.loadPixelsCommand.format)];

                        ASSERT_NO_GL_ERROR(glActiveTexture(GL_TEXTURE0));
                        ASSERT_NO_GL_ERROR(glBindTexture(type, tex.glHandle));
                        tex.format = cmd.command.loadPixelsCommand.format;
                        ASSERT_NO_GL_ERROR(glPixelStorei(GL_UNPACK_ALIGNMENT, cmd.command.loadPixelsCommand.alignment));

                        if (type == GL_TEXTURE_1D)
                        {
                            ASSERT_NO_GL_ERROR(glTexImage1D(type, 0, format.glInternalFormat, static_cast<GLuint>(cmd.command.loadPixelsCommand.width), 0, format.glFormat, glDataType, cmd.command.loadPixelsCommand.dataSource));
                        }
                        else if (type == GL_TEXTURE_2D)
                        {
                            ASSERT_NO_GL_ERROR(glTexImage2D(type, 0, format.glInternalFormat, static_cast<GLuint>(cmd.command.loadPixelsCommand.width),
                                                            static_cast<GLuint>(cmd.command.loadPixelsCommand.height), 0, format.glFormat,
                                                            glDataType, cmd.command.loadPixelsCommand.dataSource));
                        }
                        else if (type == GL_TEXTURE_3D)
                        {
                            ASSERT_NO_GL_ERROR(glTexImage3D(type, 0, format.glInternalFormat, static_cast<GLuint>(cmd.command.loadPixelsCommand.width),
                                                            static_cast<GLuint>(cmd.command.loadPixelsCommand.height), static_cast<GLuint>(cmd.command.loadPixelsCommand.depth),
                                                            0, format.glFormat, glDataType, cmd.command.loadPixelsCommand.dataSource));
                        }
                        if (tex.mipmapLevelCount > 0)
                        {
                            //make sure there is now sampler bound as that would overwrite GL_TEXTURE_MAX_LEVEL
                            ASSERT_NO_GL_ERROR(glBindSampler(0, 0));
                            ASSERT_NO_GL_ERROR(glTexParameteri(type, GL_TEXTURE_MAX_LEVEL, tex.mipmapLevelCount));
                            ASSERT_NO_GL_ERROR(glGenerateMipmap(type));
                        }

                        lastProgram = ProgramHandle();
                    }
                    else if (cmd.type == CommandBufferImpl::CommandType::UpdatePixels)
                    {
                        const CommandBufferImpl::UpdatePixelsCommand & upcmd = cmd.command.updatePixelsCommand;
                        const GLTexture & tex = m_textures.get(upcmd.handle);
                        GLenum glDataType = s_glPixelDataType[static_cast<Size>(cmd.command.updatePixelsCommand.dataType)];
                        const GLTextureFormat & format = s_glTextureFormats[static_cast<Size>(tex.format)];
                        ASSERT_NO_GL_ERROR(glBindTexture(tex.glType, tex.glHandle));
                        if (tex.glType == GL_TEXTURE_1D)
                        {
                            ASSERT_NO_GL_ERROR(glTexSubImage1D(tex.glType, 0, static_cast<GLuint>(upcmd.x), static_cast<GLuint>(upcmd.width),
                                                               format.glFormat, glDataType, upcmd.dataSource));
                        }
                        else if (tex.glType == GL_TEXTURE_2D)
                        {
                            ASSERT_NO_GL_ERROR(glTexSubImage2D(tex.glType, 0, static_cast<GLuint>(upcmd.x), static_cast<GLuint>(upcmd.y),
                                                               static_cast<GLuint>(upcmd.width), static_cast<GLuint>(upcmd.height),
                                                               format.glFormat, glDataType, upcmd.dataSource));
                        }
                        else if (tex.glType == GL_TEXTURE_3D)
                        {
                            ASSERT_NO_GL_ERROR(glTexSubImage3D(tex.glType, 0, static_cast<GLuint>(upcmd.x),
                                                               static_cast<GLuint>(upcmd.y), static_cast<GLuint>(upcmd.z),
                                                               static_cast<GLuint>(upcmd.width), static_cast<GLuint>(upcmd.height),
                                                               static_cast<GLuint>(upcmd.depth),
                                                               format.glFormat, glDataType, upcmd.dataSource));
                        }

                        if (tex.mipmapLevelCount > 0)
                        {
                            ASSERT_NO_GL_ERROR(glGenerateMipmap(tex.glType));
                        }

                        lastProgram = ProgramHandle();
                    }
                    else if (cmd.type == CommandBufferImpl::CommandType::SetProgramVariable)
                    {
                        const CommandBufferImpl::SetProgramVariableCommand & spvcmd = cmd.command.setProgramVariableCommand;
                        GLProgram & program = m_programs.get(spvcmd.program);
                        auto mit = program.uniformMap.find(buf->m_stringBuffer[spvcmd.nameStrIndex]);
                        if (mit != program.uniformMap.end())
                        {

                            const GLProgram::UniformInfo & var = program.uniforms[mit->value];
                            if (var.type == ProgramVariableType::Float32)
                            {
                                ASSERT_NO_GL_ERROR(m_glProgramUniform1f(program.glHandle, var.glLoc, spvcmd.value.floatValue));
                            }
                            else if (var.type == ProgramVariableType::Vec2f)
                            {
                                ASSERT_NO_GL_ERROR(m_glProgramUniform2fv(program.glHandle, var.glLoc, 1, &spvcmd.value.floatValue));
                            }
                            else if (var.type == ProgramVariableType::Vec3f)
                            {
                                ASSERT_NO_GL_ERROR(m_glProgramUniform3fv(program.glHandle, var.glLoc, 1, &spvcmd.value.floatValue));
                            }
                            else if (var.type == ProgramVariableType::Vec4f)
                            {
                                ASSERT_NO_GL_ERROR(m_glProgramUniform4fv(program.glHandle, var.glLoc, 1, &spvcmd.value.floatValue));
                            }
                            else if (var.type == ProgramVariableType::Matrix3f)
                            {
                                ASSERT_NO_GL_ERROR(m_glProgramUniformMatrix3fv(program.glHandle, var.glLoc, 1, false, &spvcmd.value.floatValue));
                            }
                            else if (var.type == ProgramVariableType::Matrix4f)
                            {
                                ASSERT_NO_GL_ERROR(m_glProgramUniformMatrix4fv(program.glHandle, var.glLoc, 1, false, &spvcmd.value.floatValue));
                            }
                            else
                            {
                                ret = Error(ec::BadProgramVariable, "The program type is not supported", STICK_FILE, STICK_LINE);
                                break;
                            }
                        }
                        else
                        {
                            ret = Error(ec::BadProgramVariable, "The program variable was not found", STICK_FILE, STICK_LINE);
                            break;
                        }
                    }
                    else if (cmd.type == CommandBufferImpl::CommandType::LoadVertexData)
                    {
                        const CommandBufferImpl::LoadVertexDataCommand & lvdcmd = cmd.command.loadVertexDataCommand;
                        const GLVertexBuffer & vb = m_vertexBuffers.get(lvdcmd.vertexBuffer);
                        ASSERT_NO_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, vb.glHandle));
                        ASSERT_NO_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, lvdcmd.byteCount, lvdcmd.dataSource, s_glBufferUsage[static_cast<Size>(vb.usage)]));
                    }
                    else if (cmd.type == CommandBufferImpl::CommandType::UpdateVertexData)
                    {
                        const CommandBufferImpl::UpdateVertexDataCommand & uvdcmd = cmd.command.updateVertexDataCommand;
                        const GLVertexBuffer & vb = m_vertexBuffers.get(uvdcmd.vertexBuffer);
                        ASSERT_NO_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, vb.glHandle));
                        ASSERT_NO_GL_ERROR(glBufferSubData(GL_ARRAY_BUFFER, uvdcmd.byteOffset, uvdcmd.byteCount, uvdcmd.dataSource));
                    }
                    else if (cmd.type == CommandBufferImpl::CommandType::LoadIndexData)
                    {
                        const CommandBufferImpl::LoadIndexDataCommand & lidcmd = cmd.command.loadIndexDataCommand;
                        const GLIndexBuffer & ib = m_indexBuffers.get(lidcmd.indexBuffer);
                        ASSERT_NO_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib.glHandle));
                        ASSERT_NO_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, lidcmd.byteCount, lidcmd.dataSource, s_glBufferUsage[static_cast<Size>(ib.usage)]));
                    }
                    else if (cmd.type == CommandBufferImpl::CommandType::UpdateIndexData)
                    {
                        const CommandBufferImpl::UpdateIndexDataCommand & uidcmd = cmd.command.updateIndexDataCommand;
                        const GLIndexBuffer & ib = m_indexBuffers.get(uidcmd.indexBuffer);
                        ASSERT_NO_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib.glHandle));
                        ASSERT_NO_GL_ERROR(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, uidcmd.byteOffset, uidcmd.byteCount, uidcmd.dataSource));
                    }
                    else if (cmd.type == CommandBufferImpl::CommandType::SetViewport)
                    {
                        const CommandBufferImpl::SetViewportCommand & svcmd = cmd.command.setViewportCommand;
                        ASSERT_NO_GL_ERROR(glViewport(svcmd.x, svcmd.y, svcmd.width, svcmd.height));
                    }
                    else if (cmd.type == CommandBufferImpl::CommandType::ClearBuffers)
                    {
                        const CommandBufferImpl::ClearBuffersCommand & cbcmd = cmd.command.clearBuffersCommand;
                        GLuint buffers = 0;
                        if (static_cast<Size>(cbcmd.buffersMask) & static_cast<Size>(BufferType::Color))
                        {
                            buffers |= GL_COLOR_BUFFER_BIT;
                            ASSERT_NO_GL_ERROR(glColorMask(true, true, true, true));
                            ASSERT_NO_GL_ERROR(glClearColor(cbcmd.settings.clearColor.r, cbcmd.settings.clearColor.g, cbcmd.settings.clearColor.b, cbcmd.settings.clearColor.a));
                        }
                        if (static_cast<Size>(cbcmd.buffersMask) & static_cast<Size>(BufferType::Depth))
                        {
                            buffers |= GL_DEPTH_BUFFER_BIT;
                            ASSERT_NO_GL_ERROR(glDepthMask(true));
                            ASSERT_NO_GL_ERROR(glClearDepth(cbcmd.settings.clearDepth));
                        }
                        if (static_cast<Size>(cbcmd.buffersMask) & static_cast<Size>(BufferType::Stencil))
                        {
                            buffers |= GL_STENCIL_BUFFER_BIT;
                            ASSERT_NO_GL_ERROR(glClearStencil(cbcmd.settings.clearStencil));
                            ASSERT_NO_GL_ERROR(glStencilMaskSeparate(GL_FRONT, cbcmd.settings.stencilFrontFaceWriteMask));
                            ASSERT_NO_GL_ERROR(glStencilMaskSeparate(GL_BACK, cbcmd.settings.stencilBackFaceWriteMask));
                        }

                        ASSERT_NO_GL_ERROR(glClear(buffers));

                        lastRenderState = RenderStateHandle();
                    }
                    else if (cmd.type == CommandBufferImpl::CommandType::AttachTexture)
                    {
                        const CommandBufferImpl::AttachTextureCommand & atcmd = cmd.command.attachTextureCommand;
                        GLProgram & program = m_programs.get(atcmd.program);
                        auto uit = program.uniformMap.find(buf->m_stringBuffer[atcmd.nameStrIndex]);
                        if (uit == program.uniformMap.end())
                        {
                            ret = Error(ec::BadProgramVariable, "The program variable was not found", STICK_FILE, STICK_LINE);
                            break;
                        }

                        //check if the texture is allready bound on a unit
                        bool bBreak = false;
                        Size index = -1;
                        for (Size i = 0; i < program.textureUnits.count(); ++i)
                        {
                            if (program.textureUnits[i].uniformIndex == uit->value)
                            {
                                program.textureUnits[i].texture = atcmd.texture;
                                program.textureUnits[i].sampler = atcmd.sampler;
                                index = i;
                                bBreak = true;
                                break;
                            }
                        }
                        //if not, bind it on the first free one
                        if (!bBreak)
                        {
                            for (Size i = 0; i < program.textureUnits.count(); ++i)
                            {
                                if (program.textureUnits[i].uniformIndex == -1)
                                {
                                    program.textureUnits[i].texture = atcmd.texture;
                                    program.textureUnits[i].sampler = atcmd.sampler;
                                    program.textureUnits[i].uniformIndex = uit->value;
                                    index = i;
                                    break;
                                }
                            }
                        }

                        if (index == -1)
                        {
                            //TODO: Better error code?
                            ret = Error(ec::UnspecifiedError, "All texture units are allready in use", STICK_FILE, STICK_LINE);
                            break;
                        }

                        ASSERT_NO_GL_ERROR(m_glProgramUniform1i(program.glHandle, program.uniforms[uit->value].glLoc, (GLint)index));

                        //the next draw call should rebind the program/tetures, so we invalidate the last handle here.
                        lastProgram = ProgramHandle();
                    }
                    else if (cmd.type == CommandBufferImpl::CommandType::Draw)
                    {
                        const CommandBufferImpl::DrawCommand & dcmd = cmd.command.drawCommand;
                        const GLRenderState & renderState = m_renderStates.get(dcmd.renderState);
                        const GLProgram & program = m_programs.get(dcmd.program);
                        const GLMesh & mesh = m_meshes.get(dcmd.mesh);

                        if (lastProgram != dcmd.program)
                        {
                            //activate program
                            ASSERT_NO_GL_ERROR(glUseProgram(program.glHandle));

                            //activate textures
                            for (Size i = 0; i < program.textureUnits.count(); ++i)
                            {
                                if (program.textureUnits[i].uniformIndex != -1)
                                {
                                    const GLTexture & tex = m_textures.get(program.textureUnits[i].texture);
                                    /*bool bTexAllreadyBound = false;
                                    //if this texture is a render target, check if we need to blit it
                                    if (tex.renderBuffer != RenderBufferHandle::invalidHandle)
                                    {
                                        RenderBuffer & rb = m_renderBuffers.get(tex.renderBuffer.index);
                                        finalizeRenderBufferForReading(rb);
                                        if (tex.mipmapLevelCount > 1)
                                        {
                                            ASSERT_NO_GL_ERROR(glBindSampler(0, 0));
                                            ASSERT_NO_GL_ERROR(glActiveTexture(GL_TEXTURE0 + (GLuint)i));
                                            ASSERT_NO_GL_ERROR(glBindTexture(tex.type, tex.glHandle));
                                            ASSERT_NO_GL_ERROR(glGenerateMipmap(tex.type));
                                            bTexAllreadyBound = true;
                                        }
                                    }*/

                                    const GLSampler & sampler = m_samplers.get(program.textureUnits[i].sampler);
                                    ASSERT_NO_GL_ERROR(glBindSampler((GLuint)i, sampler.glHandle));

                                    //if (!bTexAllreadyBound)
                                    {
                                        ASSERT_NO_GL_ERROR(glActiveTexture(GL_TEXTURE0 + (GLuint)i));
                                        ASSERT_NO_GL_ERROR(glBindTexture(tex.glType, tex.glHandle));
                                    }
                                }
                            }
                        }

                        //take care of the render state
                        if (lastRenderState != dcmd.renderState)
                        {
                            //set the render state
                            UInt64 diffMask = -1;
                            UInt64 stencilDiffMask = -1;
                            bool bStencilBackFaceRefDirty = true;
                            bool bStencilBackFaceMaskDirty = true;
                            bool bStencilFrontFaceRefDirty = true;
                            bool bStencilFrontFaceMaskDirty = true;
                            bool bStencilFrontFaceWriteMaskDirty = true;
                            bool bStencilBackFaceWriteMaskDirty = true;
                            if (lastRenderState.isValid())
                            {
                                const GLRenderState & lp = m_renderStates.get(lastRenderState);
                                diffMask = renderState.settings.differenceMask(lp.settings.state());
                                stencilDiffMask = renderState.settings.stencilDifferenceMask(lp.settings.stencilState().state);
                                bStencilBackFaceRefDirty = renderState.settings.stencilState().backFaceRef != lp.settings.stencilState().backFaceRef;
                                bStencilBackFaceMaskDirty = renderState.settings.stencilState().backFaceMask != lp.settings.stencilState().backFaceMask;
                                bStencilFrontFaceRefDirty = renderState.settings.stencilState().frontFaceRef != lp.settings.stencilState().frontFaceRef;
                                bStencilFrontFaceMaskDirty = renderState.settings.stencilState().frontFaceMask != lp.settings.stencilState().frontFaceMask;
                                bStencilFrontFaceWriteMaskDirty = renderState.settings.stencilState().frontFaceWriteMask != lp.settings.stencilState().frontFaceWriteMask;
                                bStencilBackFaceWriteMaskDirty = renderState.settings.stencilState().backFaceWriteMask != lp.settings.stencilState().backFaceWriteMask;
                            }

                            if (RenderStateSettings::isFlagDifferent(diffMask, RenderFlag::RF_Blending))
                            {
                                if (renderState.settings.isFlagSet(RF_Blending))
                                {
                                    ASSERT_NO_GL_ERROR(glEnable(GL_BLEND));
                                }
                                else
                                {
                                    ASSERT_NO_GL_ERROR(glDisable(GL_BLEND));
                                }
                            }

                            if (RenderStateSettings::isFlagDifferent(diffMask, RenderFlag::RF_DepthTest))
                            {
                                if (renderState.settings.isFlagSet(RF_DepthTest))
                                {
                                    ASSERT_NO_GL_ERROR(glEnable(GL_DEPTH_TEST));
                                }
                                else
                                {
                                    ASSERT_NO_GL_ERROR(glDisable(GL_DEPTH_TEST));
                                }
                            }

                            if (RenderStateSettings::isFlagDifferent(diffMask, RenderFlag::RF_Multisampling))
                            {
                                if (renderState.settings.isFlagSet(RF_Multisampling))
                                {
                                    ASSERT_NO_GL_ERROR(glEnable(GL_MULTISAMPLE));
                                }
                                else
                                {
                                    ASSERT_NO_GL_ERROR(glDisable(GL_MULTISAMPLE));
                                }
                            }

                            if (RenderStateSettings::isFlagDifferent(diffMask, RenderFlag::RF_DepthWrite))
                            {
                                ASSERT_NO_GL_ERROR(glDepthMask(renderState.settings.isFlagSet(RF_DepthWrite)));
                            }

                            if (RenderStateSettings::isFieldDifferent(diffMask, RenderFlag::RF_DepthFuncMask))
                            {
                                ASSERT_NO_GL_ERROR(glDepthFunc(s_glCompareFuncs[static_cast<Size>(renderState.settings.depthFunction())]));
                            }

                            if (RenderStateSettings::isFlagDifferent(diffMask, RenderFlag::RF_ColorWriteRed)   ||
                                    RenderStateSettings::isFlagDifferent(diffMask, RenderFlag::RF_ColorWriteGreen) ||
                                    RenderStateSettings::isFlagDifferent(diffMask, RenderFlag::RF_ColorWriteBlue)  ||
                                    RenderStateSettings::isFlagDifferent(diffMask, RenderFlag::RF_ColorWriteAlpha))
                            {
                                ASSERT_NO_GL_ERROR(glColorMask(renderState.settings.isFlagSet(RenderFlag::RF_ColorWriteRed),
                                                               renderState.settings.isFlagSet(RenderFlag::RF_ColorWriteGreen),
                                                               renderState.settings.isFlagSet(RenderFlag::RF_ColorWriteBlue),
                                                               renderState.settings.isFlagSet(RenderFlag::RF_ColorWriteAlpha)));
                            }

                            if (RenderStateSettings::isFieldDifferent(diffMask, RenderFlag::RF_ColorBlendModeMask) ||
                                    RenderStateSettings::isFieldDifferent(diffMask, RenderFlag::RF_AlphaBlendModeMask))
                            {
                                ASSERT_NO_GL_ERROR(glBlendEquationSeparate(s_glBlendModes[static_cast<Size>(renderState.settings.colorBlendMode())],
                                                   s_glBlendModes[static_cast<Size>(renderState.settings.alphaBlendMode())]));
                            }

                            if (RenderStateSettings::isFieldDifferent(diffMask, RenderFlag::RF_ColorSourceBlendFuncMask) ||
                                    RenderStateSettings::isFieldDifferent(diffMask, RenderFlag::RF_ColorDestBlendFuncMask) ||
                                    RenderStateSettings::isFieldDifferent(diffMask, RenderFlag::RF_AlphaSourceBlendFuncMask) ||
                                    RenderStateSettings::isFieldDifferent(diffMask, RenderFlag::RF_AlphaDestBlendFuncMask))
                            {
                                ASSERT_NO_GL_ERROR(glBlendFuncSeparate(s_glBlendFuncs[static_cast<Size>(renderState.settings.colorSourceBlendFunction())],
                                                                       s_glBlendFuncs[static_cast<Size>(renderState.settings.colorDestBlendFunction())],
                                                                       s_glBlendFuncs[static_cast<Size>(renderState.settings.alphaSourceBlendFunction())],
                                                                       s_glBlendFuncs[static_cast<Size>(renderState.settings.alphaDestBlendFunction())]));
                            }

                            if (RenderStateSettings::isFlagDifferent(diffMask, RF_FrontFaceClockwise))
                            {
                                ASSERT_NO_GL_ERROR(glFrontFace(renderState.settings.isFlagSet(RF_FrontFaceClockwise) ? GL_CW : GL_CCW));
                            }

                            if (RenderStateSettings::isFieldDifferent(diffMask, RenderFlag::RF_CullFaceMask))
                            {
                                if (renderState.settings.cullFace() != FaceType::None)
                                {
                                    ASSERT_NO_GL_ERROR(glEnable(GL_CULL_FACE));
                                    ASSERT_NO_GL_ERROR(glCullFace(s_glFaceType[static_cast<Size>(renderState.settings.cullFace())]));
                                }
                                else
                                {
                                    ASSERT_NO_GL_ERROR(glDisable(GL_CULL_FACE));
                                }
                            }

                            //stencil state

                            if (RenderStateSettings::isFlagDifferent(diffMask, RF_StencilTest))
                            {
                                if (renderState.settings.isFlagSet(RF_StencilTest))
                                {
                                    ASSERT_NO_GL_ERROR(glEnable(GL_STENCIL_TEST));
                                }
                                else
                                {
                                    ASSERT_NO_GL_ERROR(glDisable(GL_STENCIL_TEST));
                                }
                            }

                            if (renderState.settings.stencilState().bWriteSeparate)
                            {
                                if (bStencilBackFaceWriteMaskDirty)
                                {
                                    ASSERT_NO_GL_ERROR(glStencilMaskSeparate(GL_BACK, renderState.settings.stencilState().backFaceWriteMask));
                                }

                                if (bStencilFrontFaceWriteMaskDirty)
                                {
                                    ASSERT_NO_GL_ERROR(glStencilMaskSeparate(GL_FRONT, renderState.settings.stencilState().frontFaceWriteMask));
                                }
                            }
                            else
                            {
                                if (bStencilFrontFaceWriteMaskDirty)
                                {
                                    ASSERT_NO_GL_ERROR(glStencilMask(renderState.settings.stencilState().frontFaceWriteMask));
                                }
                            }

                            if (renderState.settings.stencilState().bFuncSeparate)
                            {
                                if (RenderStateSettings::isFieldDifferent(stencilDiffMask, SF_StencilFuncBackFaceMask) ||
                                        bStencilBackFaceRefDirty ||
                                        bStencilBackFaceMaskDirty)
                                {
                                    ASSERT_NO_GL_ERROR(glStencilFuncSeparate(GL_BACK,
                                                       s_glCompareFuncs[static_cast<Size>(renderState.settings.stencilFunctionBackFace())],
                                                       renderState.settings.stencilState().backFaceRef,
                                                       renderState.settings.stencilState().backFaceMask));
                                }

                                if (RenderStateSettings::isFieldDifferent(stencilDiffMask, SF_StencilFuncFrontFaceMask) ||
                                        bStencilFrontFaceRefDirty ||
                                        bStencilFrontFaceMaskDirty)
                                {
                                    ASSERT_NO_GL_ERROR(glStencilFuncSeparate(GL_FRONT,
                                                       s_glCompareFuncs[static_cast<Size>(renderState.settings.stencilFunctionFrontFace())],
                                                       renderState.settings.stencilState().frontFaceRef,
                                                       renderState.settings.stencilState().frontFaceMask));
                                }
                            }
                            else
                            {
                                if (RenderStateSettings::isFieldDifferent(stencilDiffMask, SF_StencilFuncFrontFaceMask) ||
                                        bStencilFrontFaceRefDirty ||
                                        bStencilFrontFaceMaskDirty)
                                {
                                    ASSERT_NO_GL_ERROR(glStencilFunc(s_glCompareFuncs[static_cast<Size>(renderState.settings.stencilFunctionFrontFace())],
                                                                     renderState.settings.stencilState().frontFaceRef,
                                                                     renderState.settings.stencilState().frontFaceMask));
                                }
                            }

                            if (renderState.settings.stencilState().bOpSeparate)
                            {
                                if (RenderStateSettings::isFieldDifferent(stencilDiffMask, SF_StencilOpSFailFrontFaceMask) ||
                                        RenderStateSettings::isFieldDifferent(stencilDiffMask, SF_StencilOpDPFailFrontFaceMask) ||
                                        RenderStateSettings::isFieldDifferent(stencilDiffMask, SF_StencilOpDPPassFrontFaceMask))
                                {
                                    ASSERT_NO_GL_ERROR(glStencilOpSeparate(GL_FRONT,
                                                                           s_glStencilOps[static_cast<Size>(renderState.settings.stencilSFailOperationFrontFace())],
                                                                           s_glStencilOps[static_cast<Size>(renderState.settings.stencilDPFailOperationFrontFace())],
                                                                           s_glStencilOps[static_cast<Size>(renderState.settings.stencilDPPassOperationFrontFace())]));
                                }

                                if (RenderStateSettings::isFieldDifferent(stencilDiffMask, SF_StencilOpSFailBackFaceMask) ||
                                        RenderStateSettings::isFieldDifferent(stencilDiffMask, SF_StencilOpDPFailBackFaceMask) ||
                                        RenderStateSettings::isFieldDifferent(stencilDiffMask, SF_StencilOpDPPassBackFaceMask))
                                {
                                    ASSERT_NO_GL_ERROR(glStencilOpSeparate(GL_BACK,
                                                                           s_glStencilOps[static_cast<Size>(renderState.settings.stencilSFailOperationBackFace())],
                                                                           s_glStencilOps[static_cast<Size>(renderState.settings.stencilDPFailOperationBackFace())],
                                                                           s_glStencilOps[static_cast<Size>(renderState.settings.stencilDPPassOperationBackFace())]));
                                }
                            }
                            else
                            {
                                if (RenderStateSettings::isFieldDifferent(stencilDiffMask, SF_StencilOpSFailFrontFaceMask))
                                {
                                    ASSERT_NO_GL_ERROR(glStencilOp(s_glStencilOps[static_cast<Size>(renderState.settings.stencilSFailOperationFrontFace())],
                                                                   s_glStencilOps[static_cast<Size>(renderState.settings.stencilDPFailOperationFrontFace())],
                                                                   s_glStencilOps[static_cast<Size>(renderState.settings.stencilDPPassOperationFrontFace())]));
                                }
                            }
                        }

                        //bind the vao
                        if (lastMesh != dcmd.mesh)
                        {
                            ASSERT_NO_GL_ERROR(glBindVertexArray(mesh.glHandle));
                        }

                        //draw
                        if (dcmd.instanceCount == 1)
                        {
                            if (!mesh.indexBuffer.isValid())
                            {
                                ASSERT_NO_GL_ERROR(glDrawArrays(s_glVertexDrawModes[static_cast<Size>(dcmd.vertexDrawMode)],
                                                                static_cast<GLuint>(dcmd.vertexOffset),
                                                                static_cast<GLuint>(dcmd.vertexCount)));

                            }
                            else
                            {
                                const GLIndexBuffer & buf = m_indexBuffers.get(mesh.indexBuffer);
                                Size bdtindex = static_cast<Size>(buf.dataType);
                                ASSERT_NO_GL_ERROR(glDrawElements(s_glVertexDrawModes[static_cast<Size>(dcmd.vertexDrawMode)],
                                                                  static_cast<GLuint>(dcmd.vertexCount),
                                                                  s_glDataTypes[bdtindex],
                                                                  (void *)(dcmd.vertexOffset * s_bufferDataTypeInfos[bdtindex].byteCount)));
                            }
                        }
                        else
                        {
                            if (!mesh.indexBuffer.isValid())
                            {
                                ASSERT_NO_GL_ERROR(glDrawArraysInstanced(s_glVertexDrawModes[static_cast<Size>(dcmd.vertexDrawMode)],
                                                   static_cast<GLuint>(dcmd.vertexOffset),
                                                   static_cast<GLuint>(dcmd.vertexCount),
                                                   static_cast<GLuint>(dcmd.instanceCount)));
                            }
                            else
                            {
                                const GLIndexBuffer & buf = m_indexBuffers.get(mesh.indexBuffer);
                                Size bdtindex = static_cast<Size>(buf.dataType);
                                ASSERT_NO_GL_ERROR(glDrawElementsInstanced(s_glVertexDrawModes[static_cast<Size>(dcmd.vertexDrawMode)],
                                                   static_cast<GLuint>(dcmd.vertexCount),
                                                   s_glDataTypes[bdtindex],
                                                   (void *)(dcmd.vertexOffset * s_bufferDataTypeInfos[bdtindex].byteCount),
                                                   static_cast<GLuint>(dcmd.instanceCount)));
                            }
                        }
                        lastMesh = dcmd.mesh;
                        lastRenderState = dcmd.renderState;
                        lastProgram = dcmd.program;
                    }
                }

                // break out if there was an error
                if (ret)
                    break;
            }

            m_commandBufferQueue.clear();

            return ret;
        }

        Error RendererImpl::submitCommandBuffer(const CommandBuffer & _buffer)
        {
            addCommandBuffer(_buffer);
            return submit();
        }
    }
}