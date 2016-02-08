#ifndef PIGMENT_RENDERSTATESETTINGS_HPP
#define PIGMENT_RENDERSTATESETTINGS_HPP

#include <Pigment/Constants.hpp>

namespace pigment
{
    class STICK_API RenderStateSettings
    {
    public:

        struct StencilState
        {
            StencilState() :
                state(0),
                frontFaceRef(0),
                frontFaceMask(0),
                backFaceRef(0),
                backFaceMask(0),
                frontFaceWriteMask(0),
                backFaceWriteMask(0),
                bWriteSeparate(false),
                bFuncSeparate(false),
                bOpSeparate(false)
            {

            }

            stick::UInt64 state;
            stick::Int32 frontFaceRef;
            stick::UInt32 frontFaceMask;
            stick::Int32 backFaceRef;
            stick::UInt32 backFaceMask;
            stick::UInt32 frontFaceWriteMask;
            stick::UInt32 backFaceWriteMask;
            bool bWriteSeparate;
            bool bFuncSeparate;
            bool bOpSeparate;
        };


        RenderStateSettings();

        RenderStateSettings & setBlending(bool _b);

        RenderStateSettings & setDepthTest(bool _b);

        RenderStateSettings & setMultisampling(bool _b);

        RenderStateSettings & setDepthWrite(bool _b);

        RenderStateSettings & setDepthFunction(CompareFunction _func);

        RenderStateSettings & setColorWrite(bool _r, bool _g, bool _b, bool _a);

        RenderStateSettings & setBlendMode(BlendMode _mode);

        RenderStateSettings & setBlendMode(BlendMode _colorMode, BlendMode _alphaMode);

        RenderStateSettings & setBlendFunction(BlendFunction _srcFunc, BlendFunction _destFunc);

        RenderStateSettings & setBlendFunction(BlendFunction _colorSrcFunc, BlendFunction _colorDestFunc,
                                       BlendFunction _alphaSrcFunc, BlendFunction _alphaDestFunc);

        RenderStateSettings & setFrontFaceDirection(FaceDirection _dir);

        RenderStateSettings & setCullFace(FaceType _faceType);

        RenderStateSettings & setStencilTest(bool _b);

        RenderStateSettings & setStencilFunction(CompareFunction _func, stick::Int32 _ref, stick::UInt32 _mask);

        RenderStateSettings & setStencilFunction(FaceType _face, CompareFunction _func, stick::Int32 _ref, stick::UInt32 _mask);

        RenderStateSettings & setStencilOperation(StencilOperation _sfail, StencilOperation _dpfail, StencilOperation _dppass);

        RenderStateSettings & setStencilOperation(FaceType _face, StencilOperation _sfail, StencilOperation _dpfail, StencilOperation _dppass);

        RenderStateSettings & setStencilWrite(stick::UInt32 _mask);

        RenderStateSettings & setStencilWrite(FaceType _face, stick::UInt32 _mask);


        bool isFlagSet(RenderFlag _flag) const;

        BlendMode colorBlendMode() const;

        BlendMode alphaBlendMode() const;

        BlendFunction colorSourceBlendFunction() const;

        BlendFunction colorDestBlendFunction() const;

        BlendFunction alphaSourceBlendFunction() const;

        BlendFunction alphaDestBlendFunction() const;

        FaceType cullFace() const;

        CompareFunction depthFunction() const;

        CompareFunction stencilFunctionFrontFace() const;

        CompareFunction stencilFunctionBackFace() const;

        StencilOperation stencilSFailOperationFrontFace() const;

        StencilOperation stencilDPFailOperationFrontFace() const;

        StencilOperation stencilDPPassOperationFrontFace() const;

        StencilOperation stencilSFailOperationBackFace() const;

        StencilOperation stencilDPFailOperationBackFace() const;

        StencilOperation stencilDPPassOperationBackFace() const;


        stick::UInt64 state() const;

        const StencilState & stencilState() const;

        stick::UInt64 differenceMask(stick::UInt64 _mask) const;

        stick::UInt64 stencilDifferenceMask(stick::UInt64 _mask) const;

        static bool isFlagDifferent(stick::UInt64 _diffMask, RenderFlag _flag);

        static bool isFieldDifferent(stick::UInt64 _diffMask, stick::UInt64 _fieldMask);


    private:

        RenderStateSettings & setFlag(RenderFlag _flag, bool _b);

        template<class T>
        RenderStateSettings & setField(RenderFlag _fieldShift, RenderFlag _fieldMask, T _value)
        {
            m_renderState &= ~_fieldMask;
            m_renderState |= (static_cast<stick::UInt64>(_value) << _fieldShift) & _fieldMask;
            return *this;
        }

        template<class T>
        T field(RenderFlag _fieldShift, RenderFlag _fieldMask) const
        {
            return static_cast<T>((m_renderState & _fieldMask) >> _fieldShift);
        }

        template<class T>
        RenderStateSettings & setStencilField(StencilFlag _fieldShift, StencilFlag _fieldMask, T _value)
        {
            m_stencilState.state &= ~_fieldMask;
            m_stencilState.state |= (static_cast<stick::UInt64>(_value) << _fieldShift) & _fieldMask;
            return *this;
        }

        template<class T>
        T stencilField(StencilFlag _fieldShift, StencilFlag _fieldMask) const
        {
            return static_cast<T>((m_stencilState.state & _fieldMask) >> _fieldShift);
        }

        stick::UInt64 m_renderState;
        StencilState m_stencilState;
    };
}

#endif //PIGMENT_RENDERSTATESETTINGS_HPP
