#ifndef APP_H
#define APP_H



class App{
    public:
        void execute()
    private:
        StepperControl steppers;
        Buzzer buz;
        GcodeParser code;
        

}

#endif