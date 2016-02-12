#ifndef PIGMENT_OPENGL_GLCOMMANDBUFFERIMPL_HPP
#define PIGMENT_OPENGL_GLCOMMANDBUFFERIMPL_HPP

#include <Pigment/CommandBuffer.hpp>

namespace pigment
{
    namespace detail
    {
        using namespace stick;

        struct STICK_LOCAL CommandBufferImpl
        {
            struct LoadPixelsCommand
            {
                TextureHandle handle;
                Size width, height, depth;
                void * dataSource;
                PixelDataType dataType;
                TextureFormat format;
                UInt32 mipmapLevelCount;
                UInt8 alignment;
            };

            struct UpdatePixelsCommand
            {
                TextureHandle handle;
                Size x, y, z, width, height, depth;
                void * dataSource;
                PixelDataType dataType;
            };

            struct SetProgramVariableCommand
            {
                ProgramHandle program;
                Size nameStrIndex;
                ProgramVariableValue value;
            };

            struct LoadVertexDataCommand
            {
                VertexBufferHandle vertexBuffer;
                Size byteCount;
                const void * dataSource;
            };

            struct UpdateVertexDataCommand
            {
                VertexBufferHandle vertexBuffer;
                Size byteOffset;
                Size byteCount;
                const void * dataSource;
            };

            struct LoadIndexDataCommand
            {
                IndexBufferHandle indexBuffer;
                Size byteCount;
                const void * dataSource;
            };

            struct UpdateIndexDataCommand
            {
                IndexBufferHandle indexBuffer;
                Size byteOffset;
                Size byteCount;
                const void * dataSource;
            };

            struct AttachTextureCommand
            {
                ProgramHandle program;
                Size nameStrIndex;
                TextureHandle texture;
                SamplerHandle sampler;
            };

            struct DrawCommand
            {
                RenderStateHandle renderState;
                ProgramHandle program;
                MeshHandle mesh;
                VertexDrawMode vertexDrawMode;
                Size vertexOffset;
                Size vertexCount;
                Size instanceCount;
            };

            struct ClearBuffersCommand
            {
                BufferType buffersMask;
                ClearBuffersSettings settings;
            };

            struct SetViewportCommand
            {
                Float32 x, y, width, height;
            };

            struct BindRenderBufferCommand
            {
                RenderBufferHandle renderBuffer;
            };

            struct ReadPixelsCommand
            {
                RenderBufferHandle renderBuffer;
                UInt32 colorTarget;
                Size left;
                Size bottom;
                Size width;
                Size height;
                ReadFormat format;
                PixelDataType dataType;
                void * destination;
            };

            enum class CommandType
            {
                LoadPixels,
                UpdatePixels,
                SetProgramVariable,
                LoadVertexData,
                UpdateVertexData,
                LoadIndexData,
                UpdateIndexData,
                AttachTexture,
                Draw,
                ClearBuffers,
                SetViewport,
                BindRenderBuffer,
                ReadPixels
            };

            union Command
            {
                LoadPixelsCommand loadPixelsCommand;
                UpdatePixelsCommand updatePixelsCommand;
                SetProgramVariableCommand setProgramVariableCommand;
                LoadVertexDataCommand loadVertexDataCommand;
                UpdateVertexDataCommand updateVertexDataCommand;
                LoadIndexDataCommand loadIndexDataCommand;
                UpdateIndexDataCommand updateIndexDataCommand;
                AttachTextureCommand attachTextureCommand;
                DrawCommand drawCommand;
                ClearBuffersCommand clearBuffersCommand;
                SetViewportCommand setViewportCommand;
                BindRenderBufferCommand bindRenderBufferCommand;
                ReadPixelsCommand readPixelsCommand;

                Command() { memset( this, 0, sizeof( Command ) ); }
            };

            struct QueuedCommand
            {
                CommandType type;
                Command command;
            };

            typedef DynamicArray<QueuedCommand> CommandQueue;
            typedef DynamicArray<String> StringBuffer;


            CommandBufferImpl(Allocator & _allocator);

            void loadPixelsRaw(const TextureHandle & _handle, Size _width, Size _height, Size _depth,
                               void * _pixels, PixelDataType _dataType, TextureFormat _format, UInt32 _mipmapLevelCount = 0, UInt8 _alignment = 4);

            void updatePixelsRaw(const TextureHandle & _handle, Size _x, Size _y, Size _z,
                                 Size _width, Size _height, Size _depth, void * _pixels, PixelDataType _dataType);

            void setProgramVariable(const ProgramHandle & _program, const String & _name, const ProgramVariableValue & _val);

            void setProgramVariablef(const ProgramHandle & _program, const String & _name, Float32 _a);

            void setProgramVariablef(const ProgramHandle & _program, const String & _name, Float32 _a, Float32 _b);

            void setProgramVariablef(const ProgramHandle & _program, const String & _name, Float32 _a, Float32 _b, Float32 _c);

            void setProgramVariablef(const ProgramHandle & _program, const String & _name, Float32 _a, Float32 _b, Float32 _c, Float32 _d);

            void setProgramVariableMatrix3f(const ProgramHandle & _program, const String & _name, Float32 * _ptr);

            void setProgramVariableMatrix4f(const ProgramHandle & _program, const String & _name, Float32 * _ptr);

            void attachTexture(const ProgramHandle & _program, const String & _name, const TextureHandle & _texture, const SamplerHandle & _sampler);

            void loadVertexDataRaw(const VertexBufferHandle & _handle, Size _byteCount, const void * _dataSource);

            void updateVertexDataRaw(const VertexBufferHandle & _handle, Size _byteOffset, Size _byteCount, const void * _dataSource);

            void loadIndexDataRaw(const IndexBufferHandle & _handle, Size _byteCount, const void * _dataSource);

            void updateIndexDataRaw(const IndexBufferHandle & _handle, Size _byteOffset, Size _byteCount, const void * _dataSource);

            void bindRenderBuffer(const RenderBufferHandle & _handle);

            void clearBuffers(BufferType _buffersMask, const ClearBuffersSettings & _settings);

            void setViewport(Float32 _x, Float32 _y, Float32 _width, Float32 _height);

            void draw(const RenderStateHandle & _renderState, const ProgramHandle & _program, const MeshHandle & _mesh, VertexDrawMode _mode, Size _vertexOffset, Size _vertexCount, Size _instanceCount = 1);

            void clear();

            Size commandCount() const;


            CommandQueue m_queue;
            StringBuffer m_stringBuffer;
        };
    }
}

#endif //PIGMENT_OPENGL_GLCOMMANDBUFFERIMPL_HPP
