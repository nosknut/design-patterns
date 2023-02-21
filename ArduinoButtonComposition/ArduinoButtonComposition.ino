struct ChangeDetector
{
    bool value = false;
    bool previousValue = false;

    bool changed = false;
    bool rising = false;
    bool falling = false;

    void update(bool newValue)
    {
        if (newValue == value)
            return;

        previousValue = value;
        value = newValue;

        changed = value != previousValue;
        rising = value && !previousValue;
        falling = !value && previousValue;
    }
};

struct Debouncer
{
    bool state = false;

    unsigned long debounceDelay = 50;
    unsigned long debounceTimer = 0;

    void update(bool newState)
    {
        if ((millis() - debounceTimer) > debounceDelay)
            return;

        if (state != newState)
            debounceTimer = millis();

        state = newState;
    }
};

struct DigitalInputPin
{
    int pin;
    bool pulldown;

    DigitalInputPin(int pin, bool pulldown)
    {
        this->pin = pin;
        this->pulldown = pulldown;
    }

    void setup()
    {
        pinMode(pin, INPUT);
    }

    bool read()
    {
        if (pulldown)
            return digitalRead(pin);
        else
            return !digitalRead(pin);
    }
};

struct Button
{
    DigitalInputPin pin;
    Debouncer debouncer;
    ChangeDetector state;

    Button(DigitalInputPin pin) : pin(pin)
    {
    }

    void setup()
    {
        pin.setup();
    }

    void update()
    {
        debouncer.update(pin.read());
        state.update(debouncer.state);
    }
};

Button button1(DigitalInputPin(2, false));
Button button2(DigitalInputPin(3, false));
Button button3(DigitalInputPin(4, false));
Button button4(DigitalInputPin(5, false));
Button button5(DigitalInputPin(6, false));
Button button6(DigitalInputPin(7, false));

void setup()
{
    Serial.begin(9600);

    button1.setup();
    button2.setup();
    button3.setup();
    button4.setup();
    button5.setup();
    button6.setup();
}

void loop()
{
    button1.update();
    button2.update();
    button3.update();
    button4.update();
    button5.update();
    button6.update();

    if (button1.state.rising)
        Serial.println("Button 1 pressed");

    if (button2.state.falling)
        Serial.println("Button 2 released");

    if (button3.state.changed)
        Serial.println("Button 3 changed");

    if (button4.state.rising)
        Serial.println("Button 4 pressed");

    if (button5.state.falling)
        Serial.println("Button 5 released");

    if (button6.state.changed)
        Serial.println("Button 6 changed");
}
