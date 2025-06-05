using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class Keypad2 : MonoBehaviour, IKeypad
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

    Global global;
    DataManager dataManager;
    RightPanel panel;
    AltTape alt;
    Hdg hdg;
    BaroBox baro;
    VsTape vs;
    SpeedTape speed;

    AudioClip tickSound;
    AudioSource audioSource;


    void deleteDigit()
    {
        currentValue = currentValue - (currentValue % 10);
        currentValue = currentValue / 10;
    }
    void updateValueDisplay()
    {
        //baro
        if (currentField == 3)
        {
            //If we're returned to 3 digits
            if (currentValue > 99 || currentValue < -99)
            {
                value.text = (currentValue / 100).ToString("0.00");
                return;
            }
        }

        value.text = (sign == -1 ? "-" : "") + Mathf.RoundToInt(currentValue).ToString();
    }
    void resetDisplay()
    {
        //IAS
        value.text = 0.ToString();
        currentValue = 0;
        sign = 1;
    }
    void enterDigit(int digit)
    {
        audioSource.PlayOneShot(tickSound);

        //if we are returned to 5 digits, do not enter any more
        if (currentValue > 9999 || currentValue < -9999)
        {
            //visual feedback
            return;
        }

        float temp = currentValue * 10;
        currentValue = temp + (currentValue >= 0 ? digit : -digit);
    }
    void confirmNumber()
    {
        Field f;
        int code;

        if (global.highlightedField == -1)
        {
            return;
        }

        switch (currentField)
        {
            case 0:
                f = speed;
                code = 0;
                break;
            case 1:
                f = alt;
                code = 1;
                break;
            case 2:
                f = vs;
                code = 2;
                break;
            case 3:
                f = baro;
                code = 3;
                break;
            default:
                f = hdg;
                code = 4;
                break;
        }

        //If it's for the baro
        if (currentField == 3)
        {
            f.editTarget(sign * currentValue / 100);
        }
        else
        {
            f.editTarget(sign * currentValue);
        }

        
        int result = f.confirmTarget();

        //failure
        if (result == 0)
        {
            ++dataManager.nbValError;

            //Reset the old target
            f.restoreOldTarget();

            //Keypad failure feedback
            StartCoroutine(global.failure(5));

            //pdf failure feedback
            StartCoroutine(global.failure(code));
        }
        //success
        else
        {
            //Put the pfd back in navigation mode
            f.toggleMode();
            
            //Remove the highlight from the PFD
            global.highlightedField = -1;

            if (SceneManager.GetActiveScene().name == "TouchGroupedKeyboard")
            {
                //Remove highlight to panel
                panel.changeButtonHighlight(false);
            }

            if (SceneManager.GetActiveScene().name == "TouchColocKeyboard")
            {
                //Hiding keypad
                global.toggleKeypadVisibility(false);
            }

            //Feedback from keypad success
            StartCoroutine(global.success(5));

            //Feedback from pfd success
            StartCoroutine(global.success(code));

            //Return the keypad to 0
            resetDisplay();
        }
    }
    bool isInside(Image image, float x, float y)
    {
        //https://stackoverflow.com/questions/40566250/unity-recttransform-contains-point

        // Get the rectangular bounding box of your UI element
        Rect rect = i.rectTransform.rect;

        // Get the left, right, top, and bottom boundaries of the rect
        float leftSide = i.rectTransform.position.x - rect.width / 2;
        float rightSide = i.rectTransform.position.x + rect.width / 2;
        float topSide = i.rectTransform.position.y + rect.height / 2;
        float bottomSide = i.rectTransform.position.y - rect.height / 2;
        
        // Check to see if the point is in the calculated bounds
        if (x >= leftSide &&
            x <= rightSide &&
            y >= bottomSide &&
            y <= topSide)
        {
            return true;
        }
        return false;
    }
    void OnMouseDown()
    {
        if (global.actionInProgress)
        {
            return;
        }

        float mouseX = Input.mousePosition.x;
        float mouseY = Input.mousePosition.y;

        if (isInside(button0, mouseX, mouseY))
        {
            ++dataManager.nbInteractionVal;
            enterDigit(0);
        }
        else if (isInside(button1, mouseX, mouseY))
        {
            ++dataManager.nbInteractionVal;
            enterDigit(1);
        }
        else if (isInside(button2, mouseX, mouseY))
        {
            ++dataManager.nbInteractionVal;
            enterDigit(2);
        }
        else if (isInside(button3, mouseX, mouseY))
        {
            ++dataManager.nbInteractionVal;
            enterDigit(3);
        }
        else if (isInside(button4, mouseX, mouseY))
        {
            ++dataManager.nbInteractionVal;
            enterDigit(4);
        }
        else if (isInside(button5, mouseX, mouseY))
        {
            ++dataManager.nbInteractionVal;
            enterDigit(5);
        }
        else if (isInside(button6, mouseX, mouseY))
        {
            ++dataManager.nbInteractionVal;
            enterDigit(6);
        }
        else if (isInside(button7, mouseX, mouseY))
        {
            ++dataManager.nbInteractionVal;
            enterDigit(7);
        }
        else if (isInside(button8, mouseX, mouseY))
        {
            ++dataManager.nbInteractionVal;
            enterDigit(8);
        }
        else if (isInside(button9, mouseX, mouseY))
        {
            ++dataManager.nbInteractionVal;
            enterDigit(9);
        }
        else if (isInside(buttonPlusMinus, mouseX, mouseY))
        {
            ++dataManager.nbInteractionVal;
            sign *= -1;
            audioSource.PlayOneShot(tickSound);
        }
        else if (isInside(buttonEnter, mouseX, mouseY))
        {
            confirmNumber();
        }
        else if (isInside(buttonDelete, mouseX, mouseY))
        {
            ++dataManager.nbInteractionVal;
            deleteDigit();
            audioSource.PlayOneShot(tickSound);
        }
        else //click out of the keypad (often a misclick)
        {
            ++dataManager.nbInteractionVal;
        }
    }

    void Start()
    {
        button0 = GameObject.Find("Canvas/Widgets/Keypad2/0 Button").GetComponent<Image>();
        button1 = GameObject.Find("Canvas/Widgets/Keypad2/1 Button").GetComponent<Image>();
        button2 = GameObject.Find("Canvas/Widgets/Keypad2/2 Button").GetComponent<Image>();
        button3 = GameObject.Find("Canvas/Widgets/Keypad2/3 Button").GetComponent<Image>();
        button4 = GameObject.Find("Canvas/Widgets/Keypad2/4 Button").GetComponent<Image>();
        button5 = GameObject.Find("Canvas/Widgets/Keypad2/5 Button").GetComponent<Image>();
        button6 = GameObject.Find("Canvas/Widgets/Keypad2/6 Button").GetComponent<Image>();
        button7 = GameObject.Find("Canvas/Widgets/Keypad2/7 Button").GetComponent<Image>();
        button8 = GameObject.Find("Canvas/Widgets/Keypad2/8 Button").GetComponent<Image>();
        button9 = GameObject.Find("Canvas/Widgets/Keypad2/9 Button").GetComponent<Image>();
        buttonPlusMinus = GameObject.Find("Canvas/Widgets/Keypad2/PlusMinus Button").GetComponent<Image>();
        buttonConfirm = GameObject.Find("Canvas/Widgets/Keypad2/Confirm Button").GetComponent<Image>();
        buttonDelete = GameObject.Find("Canvas/Widgets/Keypad2/Delete Button").GetComponent<Image>();

        value = GameObject.Find("Canvas/Widgets/Keypad2/Keypad Display/Display Value").GetComponent<Text>();
        global = GameObject.Find("Global").GetComponent<Global>();
        dataManager = GameObject.Find("Global").GetComponent<dataManager>();

        alt = GameObject.Find("Canvas/UI/Alt").GetComponent<AltTape>();
        vs = GameObject.Find("Canvas/UI/Vs").GetComponent<VsTape>();
        speed = GameObject.Find("Canvas/UI/Speed").GetComponent<SpeedTape>();
        hdg = GameObject.Find("Canvas/UI/Heading").GetComponent<Hdg>();
        baro = GameObject.Find("Canvas/UI/Baro").GetComponent<BaroBox>();


        audioSource = GameObject.Find("Audio Source").GetComponent<AudioSource>();
        tickSound = Resources.Load<AudioClip>("tick");
    }
    void Update();
}