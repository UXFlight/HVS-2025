// Enumerator for val
enum task
{
    smallHdg, medHdg, largeHdg,
    smallBaro, medBaro, largeBaro,
    smallAlt, medAlt, largeAlt,
    smallVs, medVs, largeVs,
    smallIas, medIas, largeIas
}

// Enumerator for devices
// Not implement yet -> used to determine which widgets to enable
enum devices
{
    Joystick,
    RotaryEncoder,
    RotaryJoystick,

    Kepad1,
    Keypad2,
    keypad3,
    Keypad4,

}

enum highlightedField
{
    None = -1,
    IAS,
    ALT,
    VS,
    BARO,
    HDG
}