#ifndef PIGMENT_OPENGL_GLRENDERERIMPL_HPP
#define PIGMENT_OPENGL_GLRENDERERIMPL_HPP

#include <Pigment/Constants.hpp>
#include <Pigment/CommandBuffer.hpp>
#include <Pigment/ErrorCodes.hpp>
#include <Pigment/Results.hpp>
#include <Pigment/VertexLayout.hpp>
#include <Pigment/RenderStateSettings.hpp>
#include <Pigment/Settings.hpp>
#include <Pigment/OpenGL/GLPrerequisites.hpp>
#include <Stick/String.hpp>
#include <Stick/HashMap.hpp>
#include <Stick/DynamicArray.hpp>
#include <Stick/UniquePtr.hpp>

namespace pigment
{
    namespace detail
    {
        using namespace stick;

        class CommandBufferImpl;

        struct STICK_LOCAL GLTexture
        {
            GLenum glHandle;
            Size width;
            Size height;
            Size depth;
            GLenum glType;
            TextureFormat format;
            UInt32 mipmapLevelCount;
        };

        struct STICK_LOCAL GLSampler
        {
            GLenum glHandle;
        };

        struct STICK_LOCAL GLProgram
        {
            struct UniformInfo
            {
                GLuint glLoc;
                ProgramVariableType type;
                String name;
            };

            struct TextureUnit
            {
                TextureHandle texture;
                SamplerHandle sampler;
                Size uniformIndex;
            };

            GLenum glHandle;
            DynamicArray<UniformInfo> uniforms;
            HashMap<String, Size> uniformMap;
            DynamicArray<TextureUnit> textureUnits;
        };

        struct STICK_LOCAL GLMesh
        {
            GLenum glHandle;
            IndexBufferHandle indexBuffer;
            DynamicArray<VertexBufferHandle> vertexBuffers;
        };

        struct STICK_LOCAL GLVertexBuffer
        {
            GLenum glHandle;
            VertexLayout layout;
            BufferUsage usage;
        };

        struct STICK_LOCAL GLIndexBuffer
        {
            GLenum glHandle;
            BufferUsage usage;
            BufferDataType dataType;
        };

        struct STICK_LOCAL GLRenderState
        {
            RenderStateSettings settings;
        };

        struct STICK_LOCAL RendererImpl
        {
            template<class T, class H>
            struct Storage
            {
                Storage(RendererImpl * _impl, Allocator & _alloc) :
                    m_rimpl(_impl),
                    m_array(_alloc)
                {

                }

                const T & get(const H & _handle) const
                {
                    STICK_ASSERT(_handle.m_index < m_array.size());
                    STICK_ASSERT(_handle.m_rimpl == m_rimpl);
                    return m_array[_handle.m_index];
                }

                T & get(const H & _handle)
                {
                    STICK_ASSERT(_handle.m_index < m_array.size());
                    STICK_ASSERT(_handle.m_rimpl == m_rimpl);
                    return m_array[_handle.m_index];
                }

                void remove(const H & _handle)
                {
                    m_array.remove(m_array.begin() + _handle.m_index);
                }

                H append(T && _val)
                {
                    m_array.append(forward<T>(_val));
                    return H(m_array.count() - 1, m_rimpl);
                }

                RendererImpl * m_rimpl;
                DynamicArray<T> m_array;
            };

            RendererImpl(Allocator & _alloc);

            ~RendererImpl();


            CommandBufferResult createCommandBuffer();

            void destroyCommandBuffer(CommandBuffer & _buffer);

            TextureHandleResult createTexture();

            void destroyTexture(const TextureHandle & _tex);

            SamplerHandleResult createSampler(const SamplerSettings & _settings);

            void destroySampler(const SamplerHandle & _sampler);

            ProgramHandleResult createProgram(const ProgramSettings & _settings);

            void destroyProgram(const ProgramHandle & _handle);

            MeshHandleResult createMesh(const MeshSettings & _settings);

            void destroyMesh(const MeshHandle & _mesh);

            VertexBufferHandleResult createVertexBuffer(const VertexLayout & _layout, BufferUsage _usage);

            void destroyVertexBuffer(const VertexBufferHandle & _handle);

            IndexBufferHandleResult createIndexBuffer(BufferUsage _usage);

            void destroyIndexBuffer(const IndexBufferHandle & _handle);

            RenderStateHandleResult createRenderState(const RenderStateSettings & _settings);

            void destoyRenderState(const RenderStateHandle & _handle);

            void addCommandBuffer(const CommandBuffer & _buffer);

            Error submit();

            Error submitCommandBuffer(const CommandBuffer & _buffer);


            Storage<GLTexture, TextureHandle> m_textures;
            Storage<GLSampler, SamplerHandle> m_samplers;
            Storage<GLProgram, ProgramHandle> m_programs;
            Storage<GLMesh, MeshHandle> m_meshes;
            Storage<GLVertexBuffer, VertexBufferHandle> m_vertexBuffers;
            Storage<GLIndexBuffer, IndexBufferHandle> m_indexBuffers;
            Storage<GLRenderState, RenderStateHandle> m_renderStates;
            DynamicArray<UniquePtr<CommandBuffer>> m_commandBuffers;
            DynamicArray<const CommandBufferImpl *> m_commandBufferQueue;
            Allocator * m_alloc;

            PFNGLPROGRAMUNIFORM1IPROC m_glProgramUniform1i;
            PFNGLPROGRAMUNIFORM1FPROC m_glProgramUniform1f;
            PFNGLPROGRAMUNIFORM2FVPROC m_glProgramUniform2fv;
            PFNGLPROGRAMUNIFORM3FVPROC m_glProgramUniform3fv;
            PFNGLPROGRAMUNIFORM4FVPROC m_glProgramUniform4fv;
            PFNGLPROGRAMUNIFORMMATRIX3FVPROC m_glProgramUniformMatrix3fv;
            PFNGLPROGRAMUNIFORMMATRIX4FVPROC m_glProgramUniformMatrix4fv;
        };
    }
}

#endif //PIGMENT_OPENGL_GLRENDERERIMPL_HPP
