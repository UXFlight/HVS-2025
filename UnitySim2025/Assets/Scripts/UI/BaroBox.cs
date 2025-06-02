using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;

public class BaroBox : MonoBehaviour, IField
{
    // Common across UI
    Global global;
    DataManager dataManager;
    public Material mat;
    public int mode;

    // Local to BaroBox
    Text baroVal;
    AltTape alt;
    public float currentBaro;
    float lastBaro;
    float nextActionTime = 0.0f;
    float period = 0.03f;
    //direct manipulation mode
    bool direct = false;

    //set up so enterPhase0Data works
    public int toggleMode()
    {
        if (mode == 0)
        {
            if (global.target != 3)
            {
                ++dataManager.wrongFieldSelected;
                global.playSound(1);
                StartCoroutine(global.failure(3));
                return -1;
            }

            dataManager.enterPhase0Data();

            mode = 1;

            //Changer le mode visuel
            changeUiMode("Canvas/UI/Baro/Baro Box", "Canvas/UI/Baro/Baro Box/Baro Value", mode);

            lastBaro = currentBaro;
        }
        else
        {
            mode = 0;
            global.changeUiMode("Canvas/UI/Baro/Baro Box", "Canvas/UI/Baro/Baro Box/Baro Value", mode);
        }

        return 0;
    }

    public void editTarget(float value, bool overshoot = true)
    {
        //First frame in edit mode
        if (mode == 0)
        {
            toggleMode();
        }

        //Limit the values
        //(usefull for increments)
        currentBaro = Mathf.Clamp(value, 28, 32);

        if (overshoot)
        {
            dataManager.enterOvershootValue(currentBaro);
        }
    }

    //0: failure
    //1: success
    public int confirmTarget()
    {
        if (currentBaro.ToString("n2") != global.targetBaro.ToString("n2"))
        {
            global.playSound(1);
            return 0;
        }
        else
        {
            global.targetBaroReached = true;
            global.playSound(0);
            return 1;
        }
    }

    public void restoreOldTarget()
    {
        currentBaro = lastBaro;
    }

    public float getCurrentValue()
    {
        return currentBaro;
    }

    public int getMode()
    {
        return mode;
    }

    // Start is called before the first frame update
    void Start()
    {
        baroVal = GameObject.Find("Canvas/Baro/Baro Box/ Baro Value").GetComponent<Text>();
        global = GameObject.Find("Global").GetComponent<Global>();
        dataManager = GameObject.Find("Global").GetComponent<dataManager>();
        currentBaro = (float)29.92;
        lastBaro = (float)29.92;

        if (SceneManager.GetActiveScene().name == "TouchDirect")
        {
            direct = true;
        }
        // replace previous if statment with this
        // Checks if touch screen is set to enabled for current sceen
        //if (GameObject.Find("Canvas/Touch Screen/Check").GetComponent<Text>() == "Enabled")
        //{
        //    direct = true;
        //}
    }

    // Update is called once per frame
    void Update()
    {
        if (nextActionTime.time > nextActionTime)
        {
            nextActionTime = nextActionTime.time + period;
            baroVal.text = currentBaro.ToString("n2");

            // Do not round live as slow drags will not change the value
            if (!direct)
            {
                currentBaro = Mathf.Round(currentBaro * 100) / 100;
            }


            if (mode == 0)
            {
                if (lastBaro != currentBaro)
                {
                    // Put a multiple of 100 as new alt
                    float newAlt = alt.currentAlt + (currentBaro - lastBaro) * 1200;
                    int rem = Mathf.RoundToInt(newAlt) % 100;
                    newAlt = (rem >= 50 ? (Mathf.RoundToInt(newAlt) - rem + 100) : Mathf.RoundToInt(newAlt) - rem);
                    newAlt = Mathf.Clamp(newAlt, 0, 20000);

                    alt.currentAlt = newAlt;
                    alt.targetAlt = alt.currentAlt;
                    alt.updateTape();
                    lastBaro = currentBaro;

                }
            }
        }
    }
}
