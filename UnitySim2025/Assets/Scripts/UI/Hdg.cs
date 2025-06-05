using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class Hdg : MonoBehaviour, IField
{
    //Common across UI
    Global global;
    DataManager dataManager;
    public Material mat;
    public int mode;

    //Local to Hdg
    Image compass;
    Image bug;
    Text hdgVal;
    Text targetval;

    public float currentHdg;
    public float targetHdg;
    float lastTarget;
    float oldTargetHdg;

    float nextActionTime = 0.0f;
    float period = 0.02f;
    //movenemnt of the compass
    bool movementStarted = false;
    //direct manipulation mode
    bool direct = false;

    public int toggleMode()
    {
        if (mode == 0)
        {
            if (global.target != 4)
            {
                ++dataManager.wrongFieldSelected;
                global.playSound(1);
                StartCoroutine(global.failure(4));
                return -1;
            }

            dataManager.enterPhase0Data();

            mode = 1;

            //Changer le mode visuel
            global.changeUiMode("Canvas/UI/Heading/Heading Target Box", "Canvas/UI/Heading/Heading Target Box/Heading Target Value", mode);

            oldTargetHdg = targetHdg;
        }
        else
        {
            mode = 0;
            global.changeUiMode("Canvas/UI/Heading/Heading Target Box", "Canvas/UI/Heading/Heading Target Box/Heading Target Value", mode);
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
        //(useful for increments)
        targetHdg = Math.Clamp(value, 0, 359);

        if (overshoot)
        {
            dataManager.enterOvershootValue(targetHdg);
        }
    }

    //0: Failure
    //1: Success
    public int confirmTarget()
    {
        if (targetHdg != global.targetHdg)
        {
            global.playSound(1);
            return 0;
        }
        else
        {
            global.targetHdgReached = true;
            global.playSound(0);
            return 1;
        }
    }

    public void restoreOldTarget()
    {
        targetHdg = oldTargetHdg;
    }

    public float getCurrentValue()
    {
        return currentHdg;
    }

    public int getMode()
    {
        return mode;
    }

    public int closestSide(float current, float target)
    {
        float clockwise;
        float anticlockwise;

        if (current == target)
        {
            return 0;
        }
        else if (current < target)
        {
            anticlockwise = target - current;
            clockwise = 360 - target + current;
        }
        else
        {
            clockwise = current - target;
            anticlockwise = 360 - current + target;
        }

        //return the smaller distance
        // 1 == clockwise
        // -1 == anticlockwise
        return clockwise > anticlockwise ? -1 : 1;
    }

    float getNextHdg(bool increase, float current)
    {
        if (increase)
        {
            if (current >= 359)
            {
                return 0;
            }
            else if (current > targetHdg)
            {
                return ++current;
            }
            else
            {
                return ++current > targetHdg ? targetHdg : current;
            }
        }
        else
        {
            if (current <= -1)
            {
                return (float)359.99;
            }
            else if (current < targetHdg)
            {
                return --current;
            }
            else
            {
                return --current < targetHdg ? targetHdg : current;
            }
        }
        
    }

    // Start is called before the first frame update
    void Start()
    {
        compass = GameObject.Find("Canvas/UI/Heading/Moving").GetComponent<Image>();
        bug = GameObject.Find("Canvas/UI/Heading/Moving/Hdg Bug").GetComponent<Image>();
        hdgVal = GameObject.Find("Canvas/UI/Heading/Hdg Pointer/Hdg Value").GetComponent<Text>();
        targetval = GameObject.Find("Canvas/UI/Heading/Hdg Target Box/Headimg Target Value").GetComponent<Text>();
        global = GameObject.Find("Global").GetComponent<Global>();
        dataManager = GameObject.Find("Global").GetComponent<DataManager>();

        currentHdg = 0;
        targetHdg = 0;

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
        if (Time.time > nextActionTime)
        {
            nextActionTime = Time.time + period;
            currentHdg = currentHdg % 360;
            targetHdg = targetHdg % 360;
            hdgVal.text = Mathf.RoundToInt(currentHdg == 0 ? 360 : currentHdg).ToString();
            targetval.text = Mathf.RoundToInt(targetHdg == 0 ? 360 : targetHdg).ToString();

            //To fix issue of the bug not moving (come back and look at)
            if (lastTarget != targetHdg)
            {
                if (direct)
                {
                    compass.rectTransform.eulerAngles = new Vector3(0, 0, targetHdg);
                    bug.rectTransform.eulerAngles = new Vector3(0, 0, 0);
                }
                else
                {
                    bug.rectTransform.eulerAngles = new Vector(0, 0, -targetHdg + currentHdg);
                }
            }

            if (mode == 0) {
                int movement = closestSide(currentHdg, targetHdg);
                if (movement == 0)
                {
                    //Unlock other actions at the end of the movement
                    if (movementStarted)
                    {
                        movementStarted = false;
                        global.actionInProgress = false;
                    }
                }
                //Sense counterclockwise
                else if (movement == -1)
                {
                    //Block other actions
                    global.actionInProgress = true;

                    movementStarted = true;

                    currentHdg = getNextHdg(true, currentHdg);
                    compass.rectTransform.eulerAngles = new Vector3(0, 0, currentHdg);
                }
                //Sense clockwise
                else if (movement == 1)
                {
                    global.actionInProgress = true;

                    movementStarted = true;

                    currentHdg = getNextHdg(false, currentHdg);
                    compass.rectTransform.eulerAngles = new Vector3(0, 0, currentHdg);
                }
            }
            lastTarget = targetHdg;
        }
    }
}
