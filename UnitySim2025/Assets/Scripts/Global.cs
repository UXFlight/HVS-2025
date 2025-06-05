using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class Global : MonoBehaviour
{
    /*
    Change it so that each value is stored in their
        respective class, the goal of this is so that
        global creates objects of each class 
        however the other class do not create objects 
        of global (don't create unecessary objects)
    */
    //Values to reach for the task
    public int targetSpeed = int.MinValue;
    public int targetAlt = int.MinValue;
    public int targetVs = int.MinValue;
    public float targetBaro = float.MinValue;
    public int targetHdg = int.MinValue;
    public int target = -1;

    //Values achieved
    public bool targetSpeedReached = false;
    public bool targetAltReached = false;
    public bool targetVsReached = false;
    public bool targetBaroReached = false;
    public bool targetHdgReached = false;

    //Labels of target values
    public Text hdgValue;
    public Text iasValue;
    public Text altValue;
    public Text vsValue;
    public Text baroValue;

    //PFD elements
    VsTape vs;
    SpeedTape speed;
    AltTape alt;
    Hdg hdg;
    BaroBox baro;
    public IField[] fields = new IField[5];

    //check later when debugging as this may no longer be needed
    float verticalOffset = 106; //Vertical deviation when moving pfd elements

    Image highlightBox;

    DataManager dataManager;

    //Highlighted field
    //-1: None
    //0: IAS
    //1: ALT
    //2: VS
    //3: BARO
    //4: HDG
    public int highlightedField; // replace with enum

    //Objects for the sound
    AudioClip successSound;
    AudioClip errorSound;
    AudioClip taskSound;
    AudioClip tickSound;
    AudioSource audioSource;

    //The Keypad
    Keypad2 keypad; // change this to match which keypad is in use

    //Necessary to have reference to font material
    public Material mat;

    //Action blocker
    public bool actionInProgress = false;

    public int toggleMode()
    {
        IField f;

        if (highlightedField == -1)
        {
            return -1;
        }

        switch (highlightedField)
        {
            case 0:
                f = speed;
                break;
            case 1:
                f = alt;
                break;
            case 2:
                f = vs;
                break;
            case 3:
                f = baro;
                break;
            default:
                f = hdg;
                break;
        }

        return f.toggleMode();
    }
    // changeUiMode should be changed so it acts like toggleMode
    // (custom instance in each respective class and one overall call method in global)
    // -> should reduce recursive objects
    void changeUiMode(string imagePath, string valuePath, int modeCheck)
    {
        // colors can be changed to teal or gray? dependent on values
        // use CustomColor object to set colours more clearly
        Image box = GameObject.Find(imagePath).GetComponent<Image>();
        Text value = GameObject.Find(valuePath).GetComponent<Text>();
        if (modeCheck == 0)
        {
            box.color = Color.white;
            box.material = null;
            value.color = new(63.0f / 255.0f, 220.0f / 255.0f, 241.0f / 255.0f);
        }
        else
        {
            box.color = new(63.0f / 255.0f, 220.0f / 255.0f, 241.0f / 255.0f);
            box.material = mat;
            value.color = new(61.0f / 255.0f, 61.0f / 255.0f, 61.0f / 255.0f);
        }
    }

    public void restoreOldTarget()
    {
        IField f;

        if (highlightedField == -1)
        {
            return;
        }

        int mode;

        switch (highlightedField)
        {
            case 0:
                f = speed;
                mode = speed.mode;
                break;
            case 1:
                f = alt;
                mode = alt.mode;
                break;
            case 2:
                f = vs;
                mode = vs.mode;
                break;
            case 3:
                f = baro;
                mode = baro.mode;
                break;
            default:
                f = hdg;
                mode = hdg.mode;
                break;
        }

        if (mode == 1)
        {
            f.restoreOldTarget();
        }
    }
    
    public int confirmTarget()
    {
        IField f;

        switch (highlightedField)
        {
            case 0:
                f = speed;
                break;
            case 1:
                f = alt;
                break;
            case 2:
                f = vs;
                break;
            case 3:
                f = baro;
                break;
            default:
                f = hdg;
                break;
        }

        return f.confirmTarget();
    }


    public void toggleKeypadVisibility(bool visible)
    {
        // change so that this just darkens cover? 
        // also correct coordinates
        if (visible)
        {
            keypad.GetComponent<RectTransform>().anchoredPosition = new Vector3((float)412.74, (float)-316.26);
        }
        else
        {
            keypad.GetComponent<RectTransform>().anchoredPosition = new Vector3((float)10000, (float)10000);
        }
    }

    // make enum for mode, increase available options (Keypads 1-4)
    //Keypad currently only works with Keypad1
    //0: IAS
    //1: ALT
    //2: VS
    //3: BARO
    //4: HDG
    //5: Keypad
    //6: Slider
    public IEnumerator success(int mode)
    {
        //Block other actions
        actionInProgress = true;

        Text value;
        Image box;
        Color green = new Color(0.0f / 255.0f, 255.0f / 255.0f, 89.0f / 255.0f);
        Color blue = new Color(63.0f / 255.0f, 220.0f / 255.0f, 241.0f / 255.0f);
        Color grey = new Color(61.0f / 255.0f, 61.0f / 255.0f, 61.0f / 255.0f);
        Color lightGrey = new Color(233.0f / 255.0f, 233.0f / 255.0f, 233.0f / 255.0f);

        switch (mode)
        {
            //IAS
            case 0:
                value = GameObject.Find("Canvas/UI/Speed/Speed Target Box/Speed Target Value").GetComponent<Text>();
                box = GameObject.Find("Canvas/UI/Speed/Speed Target Box").GetComponent<Image>();
                break;
            //ALT
            case 1:
                value = GameObject.Find("Canvas/UI/Alt/Alt Target Box/Alt Target Value").GetComponent<Text>();
                box = GameObject.Find("Canvas/UI/Alt/Alt Target Box").GetComponent<Image>();
                break;
            //VS
            case 2:
                value = GameObject.Find("Canvas/UI/Vs/Vs Target Box/Vs Target Value").GetComponent<Text>();
                box = GameObject.Find("Canvas/UI/Vs/Vs Target Box").GetComponent<Image>();
                break;
            //Baro
            case 3:
                value = GameObject.Find("Canvas/UI/Baro/Baro Box/Baro Value").GetComponent<Text>();
                box = GameObject.Find("Canvas/UI/Baro/Baro Box").GetComponent<Image>();
                break;
            //HDG
            case 4:
                value = GameObject.Find("Canvas/UI/Heading/Hdg Target Box/Hdg Target Value").GetComponent<Text>();
                box = GameObject.Find("Canvas/UI/Heading/Hdg Target Box").GetComponent<Image>();
                break;
            //Keypad
            case 5:
                value = GameObject.Find("Canvas/Widgets/Keypad1/Keypad Display/Display Value").GetComponent<Text>();
                box = GameObject.Find("Canvas/Widgets/Keypad1/Keypad Display/Display Bg").GetComponent<Image>();
                break;
            //Slider
            default:
                value = GameObject.Find("Canvas/Widgets/Slider Interface/Slider Display/Slider Value").GetComponent<Text>();
                box = GameObject.Find("Canvas/Widgets/Slider Interface/Slider Display/Slider Display Bg").GetComponent<Image>();
                break;
        }

        Color initialColorVal = value.color;
        Color initialColorBox = box.color;

        bool hasMat = true;



        if (box.material != mat)
        {
            box.material = mat;

            if (mode == 5 || mode == 6)
            {
                initialColorBox = lightGrey;
            }
            else
            {
                initialColorBox = grey;
            }

            hasMat = false;
        }

        box.color = green;

        value.color = Color.black;

        float fadeTime = (float)0.5;
        float counter = 0;

        Color currentColorVal = value.color;
        Color currentColorBox = box.color;

        while (counter < fadeTime)
        {
            counter += Time.deltaTime;
            value.color = Color.Lerp(Color.black, initialColorVal, counter / fadeTime);
            box.color = Color.Lerp(green, initialColorBox, counter / fadeTime);
            yield return null;
        }

        //See if this bug
        //TODO: Set the current editing mode, after the while
        if (hasMat == false)
        {
            box.color = Color.white;
            box.material = null;
        }

        //Unlock other actions
        actionInProgress = false;
    }

    //use same enum as success function
    //0: IAS
    //1: ALT
    //2: VS
    //3: BARO
    //4: HDG
    //5: Keypad
    //6: Slider
    public IEnumerator failure(int mode)
    {
        //Bloquer les autres actions
        actionInProgress = true;

        Text value;
        Image box;
        Color blue = new Color(63.0f / 255.0f, 220.0f / 255.0f, 241.0f / 255.0f);
        Color grey = new Color(61.0f / 255.0f, 61.0f / 255.0f, 61.0f / 255.0f);
        Color lightGrey = new Color(233.0f / 255.0f, 233.0f / 255.0f, 233.0f / 255.0f);
        Color red = new Color(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);

        switch (mode)
        {
            //IAS
            case 0:
                value = GameObject.Find("Canvas/UI/Speed/Speed Target Box/Speed Target Value").GetComponent<Text>();
                box = GameObject.Find("Canvas/UI/Speed/Speed Target Box").GetComponent<Image>();
                break;
            //ALT
            case 1:
                value = GameObject.Find("Canvas/UI/Alt/Alt Target Box/Alt Target Value").GetComponent<Text>();
                box = GameObject.Find("Canvas/UI/Alt/Alt Target Box").GetComponent<Image>();
                break;
            //VS
            case 2:
                value = GameObject.Find("Canvas/UI/Vs/Vs Target Box/Vs Target Value").GetComponent<Text>();
                box = GameObject.Find("Canvas/UI/Vs/Vs Target Box").GetComponent<Image>();
                break;
            //Baro
            case 3:
                value = GameObject.Find("Canvas/UI/Baro/Baro Box/Baro Value").GetComponent<Text>();
                box = GameObject.Find("Canvas/UI/Baro/Baro Box").GetComponent<Image>();
                break;
            //HDG
            case 4:
                value = GameObject.Find("Canvas/UI/Heading/Hdg Target Box/Hdg Target Value").GetComponent<Text>();
                box = GameObject.Find("Canvas/UI/Heading/Hdg Target Box").GetComponent<Image>();
                break;
            //Keypad
            case 5:
                value = GameObject.Find("Canvas/Widgets/Keypad1/Keypad Display/Display Value").GetComponent<Text>();
                box = GameObject.Find("Canvas/Widgets/Keypad1/Keypad Display/Display Bg").GetComponent<Image>();
                break;
            //Slider
            default:
                value = GameObject.Find("Canvas/Widgets/Slider Interface/Slider Display/Slider Value").GetComponent<Text>();
                box = GameObject.Find("Canvas/Widgets/Slider Interface/Slider Display/Slider Display Bg").GetComponent<Image>();
                break;
        }

        Color initialColorVal = value.color;
        Color initialColorBox = box.color;

        bool hasMat = true;

        if (box.material != mat)
        {
            box.material = mat;

            if (mode == 5 || mode == 6)
            {
                initialColorBox = lightGrey;
            }
            else
            {
                initialColorBox = grey;
            }

            hasMat = false;
        }

        if (mode == 3)
        {
            //Debug.Log(hasMat);
        }

        box.color = red;
        value.color = Color.white;

        Color currentColorVal = value.color;
        Color currentColorBox = box.color;

        float fadeTime = (float)0.5;
        float counter = 0;

        while (counter < fadeTime)
        {
            counter += Time.deltaTime;
            value.color = Color.Lerp(currentColorVal, initialColorVal, counter / fadeTime);
            box.color = Color.Lerp(currentColorBox, initialColorBox, counter / fadeTime);
            yield return null;
        }

        //See if this bug
        if (!hasMat)
        {
            box.color = Color.white;
            box.material = null;
        }

        //Unlock the other actions
        actionInProgress = false;
    }

    // use enum? (I don't think so as there are only two options)
    //0: succes
    //1: failure
    public void playSound(int sound)
    {
        AudioClip clip;
        if (sound == 0)
        {
            clip = successSound;
        }
        else if (sound == 1)
        {
            clip = errorSound;
        }
        else
        {
            clip = taskSound;
        }

        audioSource.PlayOneShot(clip);
    }

    // could loop through fields 
    // (something like for (int i=0, i<length(fields), i++){if (fields[i] == 1) fields[i].toggleMode{}})
    // to reduce space and increase readability
    public void resetPfdModes()
    {
        if (speed.mode == 1)
        {
            speed.toggleMode();
        }
        if (alt.mode == 1)
        {
            alt.toggleMode();
        }
        if (vs.mode == 1)
        {
            vs.toggleMode();
        }
        if (baro.mode == 1)
        {
            baro.toggleMode();
        }
        if (hdg.mode == 1)
        {
            hdg.toggleMode();
        }
    }

    //NOTE: if switchin to enumerators for highlightedField these two functions will not work 
    // (enums can't be incremented)
    // possible solution -> leave highlighedField as int and type cast enums into it and to compare with it?
    public void highlightNextField()
    {
        //Skip the hdg if we're in nav mode
        highlightedField = highlightedField == 4 ? 0 : highlightedField + 1;
    }

    public void highlightPreviousField()
    {
        //Skip the hdg if we're in nav mode
        highlightedField = highlightedField == 0 ? 4 : highlightedField - 1;
    }

    void drawHighlight()
    {
        Vector3 position = Vector3.zero;
        float width = 0;

        // double check locations and widths
        switch (highlightedField)
        {
            //None
            case -1:
                position = new Vector3((float)-2000, (float)2000, 0);
                width = (float)108.01;
                break;
            //IAS
            case 0:
                position = new Vector3((float)-338.3, (float)292.25 + verticalOffset, 0);
                width = (float)108.01;
                break;
            //ALT
            case 1:
                position = new Vector3((float)329.7906, (float)292.25 + verticalOffset, 0);
                width = (float)130.5;
                break;
            //VS
            case 2:
                position = new Vector3((float)454.5, (float)292.25 + verticalOffset, 0);
                width = (float)108.01;
                break;
            //BARO
            case 3:
                position = new Vector3((float)329.7906, (float)-170.5 + verticalOffset, 0);
                width = (float)130.5;
                break;
            //HDG
            case 4:
                position = new Vector3(-281, -289 + verticalOffset, 0);
                width = (float)108.01;
                break;
        }

        highlightBox.rectTransform.anchoredPosition = position;
        highlightBox.rectTransform.SetSizeWithCurrentAnchors(RectTransform.Axis.Horizontal, width);
    }

    //field: see above
    //direction: -1 For negative, 1 for positive
    //size: 0 for small, 1 for large
    public void incrementValue(int field, int direction, int size)
    {
        float increment;
        switch (field)
        {
            //IAS
            case 0:
                increment = (size == 0 ? 1 : 10) * direction;
                //if the target speed is zero, take the current value
                speed.editTarget(speed.targetSpeed == float.MinValue ? speed.currentSpeed + increment : speed.targetSpeed + increment);
                break;
            //ALT
            case 1:
                increment = (size == 0 ? 100 : 1000) * direction;
                alt.editTarget(alt.targetAlt + increment);
                break;
            //VS
            case 2:
                increment = (size == 0 ? 100 : 1000) * direction;
                //if the vs target is zero, take the current value
                vs.editTarget(vs.targetVs == float.MinValue ? vs.currentVs + increment : vs.targetVs + increment);
                break;
            //BARO
            case 3:
                increment = (float) (size == 0 ? 0.01 : 0.1) * direction;
                baro.editTarget(baro.currentBaro + increment);
                break;
            //HDG
            case 4:
                increment = (size == 0 ? 1 : 10) * direction;
                float temp = hdg.targetHdg + increment;
                if (temp < 0)
                {
                    temp = 360 + temp;
                }
                else if (temp >= 360)
                {
                    temp = temp % 360;
                }
                hdg.editTarget(temp);
                break;
        }
    }


    public void setValue(IField field, float value)
    {
        field.editTarget(value);
    }

    //direction: -1 for negative, 1 for positive
    //size: 0 for small, 1 for large
    public void getNextMovement(int direction, int size, bool log = false)
    {
        switch (highlightedField)
        {
            //IAS
            case 0:
                if (speed.mode == 0)
                {
                    if (direction == 1)
                    {
                        highlightNextField();
                    }
                    else
                    {
                        highlightPreviousField();
                    }
                }
                else
                {
                    incrementValue(0, direction, size);

                    if (log)
                    {
                        ++dataManager.nbInteractionVal;
                    }

                    //audioSource.PlayOneShot(tickSound);
                }
                break;
            //ALT
            case 1:
                if (alt.mode == 0)
                {
                    if (direction == 1)
                    {
                        highlightNextField();
                    }
                    else
                    {
                        highlightPreviousField();
                    }
                }
                else
                {
                    incrementValue(1, direction, size);

                    if (log)
                    {
                        ++dataManager.nbInteractionVal;
                    }
                    //audioSource.PlayOneShot(tickSound);
                }
                break;
            //VS
            case 2:
                if (vs.mode == 0)
                {
                    if (direction == 1)
                    {
                        highlightNextField();
                    }
                    else
                    {
                        highlightPreviousField();
                    }
                }
                else
                {
                    incrementValue(2, direction, size);

                    if (log)
                    {
                        ++dataManager.nbInteractionVal;
                    }

                    //audioSource.PlayOneShot(tickSound);
                }
                break;
            //BARO
            case 3:
                if (baro.mode == 0)
                {
                    if (direction == 1)
                    {
                        highlightNextField();
                    }
                    else
                    {
                        highlightPreviousField();
                    }
                }
                else
                {
                    incrementValue(3, direction, size);

                    if (log)
                    {
                        ++dataManager.nbInteractionVal;
                    }

                    //audioSource.PlayOneShot(tickSound);
                }
                break;
            //HDG
            case 4:
                if (hdg.mode == 0)
                {
                    if (direction == 1)
                    {
                        highlightNextField();
                    }
                    else
                    {
                        highlightPreviousField();
                    }
                }
                else
                {
                    incrementValue(4, direction, size);

                    if (log)
                    {
                        ++dataManager.nbInteractionVal;
                    }

                    //audioSource.PlayOneShot(tickSound);
                }
                break;
        }
    }

    // Start is called before the first frame update
    void Start()
    {
        vs = GameObject.Find("Canvas/UI/Vs").GetComponent<VsTape>();
        speed = GameObject.Find("Canvas/UI/Speed").GetComponent<SpeedTape>();
        alt = GameObject.Find("Canvas/UI/Alt").GetComponent<AltTape>();
        hdg = GameObject.Find("Canvas/UI/Heading").GetComponent<Hdg>();
        baro = GameObject.Find("Canvas/UI/Baro").GetComponent<BaroBox>();
        highlightBox = GameObject.Find("Canvas/UI/Highlight Box").GetComponent<Image>();

        dataManager = GameObject.Find("Global").GetComponent<DataManager>();

        fields[0] = speed;
        fields[1] = alt;
        fields[2] = vs;
        fields[3] = baro;
        fields[4] = hdg;

        hdgValue = GameObject.Find("Canvas/Left Panel/Values/Hdg Value").GetComponent<Text>();
        iasValue = GameObject.Find("Canvas/Left Panel/Values/Ias Value").GetComponent<Text>(); 
        altValue = GameObject.Find("Canvas/Left Panel/Values/Alt Value").GetComponent<Text>(); 
        vsValue = GameObject.Find("Canvas/Left Panel/Values/Vs Value").GetComponent<Text>(); 
        baroValue = GameObject.Find("Canvas/Left Panel/Values/Baro Value").GetComponent<Text>();

        audioSource = GameObject.Find("Audio Source").GetComponent<AudioSource>();
        successSound = Resources.Load<AudioClip>("success");
        errorSound = Resources.Load<AudioClip>("error");
        taskSound = Resources.Load<AudioClip>("task");
        tickSound = Resources.Load<AudioClip>("tick");

        
        if (SceneManager.GetActiveScene().name == "TouchGroupedKeyboard" || SceneManager.GetActiveScene().name == "TouchColocKeyboard")
        {
            keypad = GameObject.Find("Canvas/Widgets/Keypad1").GetComponent<Keypad2>();
        }
        // replace previous if statment with this
        // Checks if touch screen is set to enabled for current sceen
        //replace Check with selected keyboard
        //if (GameObject.Find("Canvas/Touch Screen/Check").GetComponent<Text>() == "Enabled")
        //{
        //    keypad = GameObject.Find("Canvas/Widgets/Keypad1").GetComponent<Keypad2>();
        //} 

        highlightedField = 0;

        //test
        /*
        targetAlt = 10000;
        targetSpeed = 180;
        targetVs = -900;
        targetBaro = (float)29.82;
        targetHdg = 150;
        */
    }

    // Update is called once per frame
    void Update()
    {
        //Updating targets
        //checkTargets();

        //Show targets
        //writeTargets();

        //Change the highlight
        drawHighlight();
    }
}
