
#ifndef __INT32Q16_HPP
#define __INT32Q16_HPP


class Int32Q16_t
{
    // Constructors
public:
    Int32Q16_t(void) {
        this->value = 0;
    }
    Int32Q16_t(int32_t value_p) {
        this->value = value_p;
    }
    Int32Q16_t(uint32_t value_p) {
        this->value = (int32_t)value_p;
    }
    Int32Q16_t(int64_t value_p) {
        this->value = (int32_t)value_p;
    }
    Int32Q16_t(uint64_t value_p) {
        this->value = (int32_t)value_p;
    }
    Int32Q16_t(double value_p) {
        this->value = (int32_t)value_p;
    }
    void operator = (const Int32Q16_t &value_p) {
        this->value = value_p.value;
    }

    // Operators overloading - casting
    operator int8_t() const {
        return (int8_t)(this->value);
    }
    operator uint8_t() const {
        return (uint8_t)(this->value);
    }
    operator float() const {
        return this->value;
    }
    operator int32_t() const {
        return this->value;
    }
    operator int16_t() const {
        return (int16_t)(this->value);
    }
    operator uint16_t() const {
        return (uint16_t)(this->value);
    }
    operator uint32_t() const {
        return (uint32_t)(this->value);
    }
    operator int64_t() const {
        return (int64_t)(this->value);
    }
    operator uint64_t() const {
        return (uint64_t)(this->value);
    }

    // Operators overloading - arithmetic (+)

    // Int32Q16_t
    friend Int32Q16_t operator+(const Int32Q16_t &a, const Int32Q16_t &b) {
        return Int32Q16_t(a.value + b.value);
    }

    // float
    friend Int32Q16_t operator+(const Int32Q16_t &a, const float b) {
        return Int32Q16_t(a.value + b);
    }
    friend Int32Q16_t operator+(const float a, const Int32Q16_t &b) {
        return Int32Q16_t(a + b.value);
    }

    // int32_t
    friend Int32Q16_t operator+(const Int32Q16_t &a, const int32_t b) {
        return Int32Q16_t(a.value + b);
    }
    friend Int32Q16_t operator+(const int32_t a, const Int32Q16_t &b) {
        return Int32Q16_t(a + b.value);
    }

    // uint32_t
    friend Int32Q16_t operator+(const Int32Q16_t &a, const uint32_t b) {
        return Int32Q16_t(a.value + static_cast<int32_t>(b));
    }
    friend Int32Q16_t operator+(const uint32_t a, const Int32Q16_t &b) {
        return Int32Q16_t(static_cast<int32_t>(a) + b.value);
    }

    // int64_t
    friend Int32Q16_t operator+(const Int32Q16_t &a, const int64_t b) {
        return Int32Q16_t(a.value + static_cast<int32_t>(b));
    }
    friend Int32Q16_t operator+(const int64_t a, const Int32Q16_t &b) {
        return Int32Q16_t(static_cast<int32_t>(a) + b.value);
    }

    // uint64_t
    friend Int32Q16_t operator+(const Int32Q16_t &a, const uint64_t b) {
        return Int32Q16_t(a.value + static_cast<int32_t>(b));
    }
    friend Int32Q16_t operator+(const uint64_t a, const Int32Q16_t &b) {
        return Int32Q16_t(static_cast<int32_t>(a) + b.value);
    }

    int32_t getValue(void) {
        return this->value;
    }

private:
    int32_t	value;
};

#endif
