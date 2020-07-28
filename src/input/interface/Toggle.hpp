#pragma once

class Toggle
{
public:

    explicit Toggle(bool value) : _value(value)
    {}

    // Returns true on value changed.
    bool feed(bool value)
    {
        if (_value != value)
        {
            _value = value;
            _changed = true;
        }
        else
        {
            _changed = false;
        }

        return _changed;
    }

    // Returns currently held value.
    bool value() const { return _value; };

    // True if last feed changed currently held value.
    bool changed() const { return _changed; };

private:

    friend class Input;
    void reset_changed() { _changed = false; }

    bool _value = false;
    bool _changed = false;
};
