/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_crtp_130917145531
#define KSERGEY_crtp_130917145531

namespace plusone {

/**
 * Curiously recurring template pattern helper.
 * @see http://www.fluentcpp.com/2017/05/12/curiously-recurring-template-pattern/
 * @see https://www.fluentcpp.com/2017/05/19/crtp-helper/
 */
template< class Impl >
class crtp
{
protected:
    crtp() = default;
    ~crtp() = default;

    /** @return Const reference for implementation class. */
    const Impl& impl() const noexcept
    {
        return static_cast< const Impl& >(*this);
    }

    /** @return Reference for implementation class. */
    Impl& impl() noexcept
    {
        return static_cast< Impl& >(*this);
    }
};

} /* namespace plusone */

#endif /* KSERGEY_crtp_130917145531 */
