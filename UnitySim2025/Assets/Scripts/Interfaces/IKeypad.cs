using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public interface IKeypad
{
    Image button1;
    Image button2;
    Image button3;
    Image button4;
    Image button5;
    Image button6;
    Image button7;
    Image button8;
    Image button9;
    Image button0;
    Image buttonPlusMinus;
    Image buttonEnter;
    Image buttonDelete;

    Text value;

    float currentValue;
    int currentField;
    int sign = 1;

    void deleteDigit();
    void updateValueDisplay();
    void resetDisplay();
    void enterDigit(int digit);
    void confirmNumber();
    bool isInside(Image image, float x, float y);
    void OnMouseDown();

    void Start();
    void Update();
}