using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using UnityEngine.EventSystems;

// needs updating, likely doesn't work yet
// can use _slider.SetEnabled(true/false) to set state
//
// for some reason _slider.value = is not working

public class SliderInterface : MonoBehaviour
{
    Slider _slider;
    public Text sliderText;

    Text labelMin;
    Text labelMidMin;
    Text labelMid;
    Text labelMidMax;
    Text labelMax;

    float currentValue;
    int currentField;
    int lastField;

    Global global;

    // updates displayed value based on slider position 
    // attached to slider
    public void updateDisplayValue()
    {
        float round;
        if (currentField == 1) // if Alt -- remove if we want alt to the ones place
        {
            round = (float)Math.Round(_slider.value / 100) * 100;
        }
        else if (currentField == 3) // if Baro
        {
            round = (float)Math.Round(_slider.value, 2);
        }
        else
        {
            round = (float)Math.Round(_slider.value);
        }
        _slider.value = round;
        sliderText.text = _slider.value.ToString();
        updateCurrentValue();
    }

    // writes labels corresponding to each field on slider
    void writeScaleLabels()
    {
        switch (currentField)
        {
            //IAS
            case 0:
                labelMin.text = "0";
                labelMidMin.text = "100";
                labelMid.text = "200";
                labelMidMax.text = "300";
                labelMax.text = "400";
                break;
            //ALT
            case 1:
                labelMin.text = "0";
                labelMidMin.text = "5k";
                labelMid.text = "10k";
                labelMidMax.text = "15k";
                labelMax.text = "20k";
                break;
            //VS
            case 2:
                labelMin.text = "-2k";
                labelMidMin.text = "-1k";
                labelMid.text = "0";
                labelMidMax.text = "1k";
                labelMax.text = "2k";
                break;
            //BARO
            case 3:
                labelMin.text = "28";
                labelMidMin.text = "29";
                labelMid.text = "30";
                labelMidMax.text = "31";
                labelMax.text = "32";
                break;
            //HDG
            case 4:
                labelMin.text = "1";
                labelMidMin.text = "90";
                labelMid.text = "180";
                labelMidMax.text = "270";
                labelMax.text = "360";
                break;
            //Nothing selected
            case -1:
                labelMin.text = "XXX";
                labelMidMin.text = "XXX";
                labelMid.text = "XXX";
                labelMidMax.text = "XXX";
                labelMax.text = "XXX";
                break;
        }
    }

    //update slider and set its ranges
    void updateSlider()
    {
        switch (currentField)
        {
            //IAS
            case 0:
                _slider.maxValue = 400;
                _slider.minValue = 0;
                _slider.wholeNumbers = true;
                break;
            //ALT
            case 1:
                _slider.maxValue = 20000;
                _slider.minValue = 0;
                _slider.wholeNumbers = true;
                break;
            //VS
            case 2:
                _slider.maxValue = 2000;
                _slider.minValue = -2000;
                _slider.wholeNumbers = true;
                break;
            //BARO
            case 3:
                _slider.maxValue = 32;
                _slider.minValue = 28;
                _slider.wholeNumbers = false;
                break;
            //HDG
            case 4:
                _slider.maxValue = 360;
                _slider.minValue = 1;
                _slider.wholeNumbers = true;
                break;
            //Nothing selected
            case -1:
                _slider.maxValue = 1;
                _slider.minValue = 0;
                _slider.wholeNumbers = false;
                _slider.value = 0;
                break;
        }
    }

    // called in global, sets slider value from current field values
    public void setupSlider(float newValue, int highlightedField)
    {
        currentField = highlightedField;
        if ((currentField == 4) && (newValue == 0))
        {
            newValue = 360;
        }
        updateSlider();
        writeScaleLabels();
        updateDisplayValue();
        _slider.value = newValue;
    }

    // updates current value based on slider position
    void updateCurrentValue()
    {
        currentValue = (float)_slider.value;
    }

    //increments slider value based on selected field, 
    // attached to Plus Button and Minus Button
    public void incrementValue() 
    {
        //direction: -1 for negative, 1 for positive
        //size: 0 for small, 1 for large    *********** figure out how this should be chosen
        string directionInput = EventSystem.current.currentSelectedGameObject.GetComponentInChildren<Text>().text;
        int direction = 0;
        float increment = 0;
        int size = 0; // figure out how size should be set

        if (directionInput == "+")
        {
            direction = 1;
        }
        else if (directionInput == "-")
        {
            direction = -1;
        }

        switch (currentField)
        {
            //IAS
            case 0:
                increment = (size == 0 ? 1 : 10) * direction;
                break;
            //ALT
            case 1:
                increment = (size == 0 ? 100 : 1000) * direction;
                break;
            //VS
            case 2:
                increment = (size == 0 ? 100 : 1000) * direction;
                break;
            //BARO
            case 3:
                increment = (float)(size == 0 ? 0.01 : 0.1) * direction;
                break;
            //HDG
            case 4:
                increment = (size == 0 ? 1 : 10) * direction;
                break;
        }

        _slider.value += increment;
        sliderText.text = _slider.value.ToString();
    }

    public float getSliderValue()
    {
        return (float)_slider.value;
    }

    // Start is called before the first frame update
    void Start()
    {
        labelMin = GameObject.Find("Canvas/Widgets/Slider Interface/Slider/Label Min").GetComponent<Text>();
        labelMidMin = GameObject.Find("Canvas/Widgets/Slider Interface/Slider/Label MidMin").GetComponent<Text>();
        labelMid = GameObject.Find("Canvas/Widgets/Slider Interface/Slider/Label Mid").GetComponent<Text>();
        labelMidMax = GameObject.Find("Canvas/Widgets/Slider Interface/Slider/Label MidMax").GetComponent<Text>();
        labelMax = GameObject.Find("Canvas/Widgets/Slider Interface/Slider/Label Max").GetComponent<Text>();

        global = GameObject.Find("Global").GetComponent<Global>();

        _slider = GameObject.Find("Canvas/Widgets/Slider Interface/Slider").GetComponent<Slider>();
        sliderText = GameObject.Find("Canvas/Widgets/Slider Interface/Slider Display/Slider Value").GetComponent<Text>();
    }
} 
