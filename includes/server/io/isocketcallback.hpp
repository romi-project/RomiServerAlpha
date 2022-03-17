// The Romi Project
//
// @ File   :  isocketcallback.hpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-17 14:20:51
//

/*
**  Romi Socket Callback Interface
**
**    EventPort에서 이벤트가 발생 될 때,
**    그에 맞는 적절히 처리될 함수를 미리 정의하는 인터페이스 클래스 입니다.
*/

#pragma once

class RISocketCallback
{
public:
    RISocketCallback() = default;
    virtual ~RISocketCallback() = default;
    RISocketCallback(const RISocketCallback&) = default;
    RISocketCallback(RISocketCallback&&) = default;
    RISocketCallback& operator= (const RISocketCallback&) = default;
    RISocketCallback& operator= (RISocketCallback&&) = default;

    virtual void OnSend() = 0;
    virtual void OnReceive() = 0;
};
