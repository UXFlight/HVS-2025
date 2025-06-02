using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SpeedTape : MonoBehaviour, IField
{
    // Common across UI
    Global global;
    DataManager dataManager;
    public Material mat;
    public int mode;

    // Local to SpeedTape
    Image tapel
    public Image bug;
    Text speedVal;
    Text targetVal;

    float maxHeight = (float)1491;
    float minBugHeight = (float)-1441.2;

    public float currentSpeed;
    public float targetSpeed;
    float oldTargetSpeed;

    float nextActionTime = 0.0f;
    float period = 0.001f;
    float changeSpeed = (float)5;

    float pixelToSpeed(float pix)
    {
        return (float)((maxHeight - pix) / 7.2);
    }

    public float speedToPixel(float speed)
    {
        return (float)(maxHeight - (speed * 7.2));
    }

    public float bugPosition(float speed)
    {
        return (float)(minBugHeight + (speed * 7.2));
    }

    void hideBug()
    {
        bug.rectTransform.anchoredPosition = new Vector3(bug.rectTransform.anchoredPosition.x, -10000);
    }

    public int toggleMode()
    {
        if (mode == 0)
        {
            if (global.target != 0)
            {
                ++dataManager.wrongFieldSelected;
                global.playSound(1);
                StartCoroutine(global.failure(0));
                return -1;
            }

            dataManager.enterPhase0Data();

            mode = 1;

            //Changer le mode visuel
            global.changeUiMode("Canvas/UI/Speed/Speed Target Box", "Canvas/UI/Speed/Speed Target Box/Speed Target Value", mode);

            oldTargetSpeed = targetSpeed;
        }
        else
        {
            //Faire un changement en 1.5 secondes
            changeSpeed = Mathf.Abs(targetSpeed - currentSpeed) / (1.5f / Time.deltaTime);

            mode = 0;
            global.changeUiMode("Canvas/UI/Speed/Speed Target Box", "Canvas/UI/Speed/Speed Target Box/Speed Target Value", mode);
        }

        return 0;
    }

    public void editTarget(float value, bool overshoot = true)
    {
        //First frame in edition mode
        if (mode == 0)
        {
            toggleMode();
        }

        //Limit the values
        //(useful for increments)
        targetSpeed = Mathf.Clamp(value, 0, 400);

        if (overshoot)
        {
            dataManager.enterOvershootValue(targetSpeed);
        }
    }

    //0: Failure
    //1: Succes
    public int confirmTarget()
    {
        if (targetSpeed != global.targetSpeed)
        {
            global.playSound(1);
            return 0;
        }
        else
        {
            global.targetSpeedReached = true;
            global.playSound(0);
            return 1;
        }
    }

    public void restoreOldTarget()
    {
        targetSpeed = oldTargetSpeed;
    }

    public float getCurrentValue()
    {
        return currentSpeed;
    }

    public int getMode()
    {
        return mode;
    }

    // Start is called before the first frame update
    void Start()
    {
        tape = GameObject.Find("Canvas/UI/Speed/Moving").GetComponent<Image>();
        bug = GameObject.Find("Canvas/UI/Speed/Moving/Speed Bug").GetComponent<Image>();
        speedVal = GameObject.Find("Canvas/UI/Speed/Speed Pointer/ Speed Value").GetComponent<Text>();
        targetVal = GameObject.Find("Canvas/UI/Speed/Speed Target Box/Speed Target Value").GetComponent<Text>();
        global = GameObject.Find("Global").GetComponent<Global>();
        dataManager = GameObject.Find("Global").GetComponent<DataManager>();

        currentSpeed = 160;
        targetSpeed = float.MinValuel

        tape.rectTransform.anchoredPosition = new Vector3(tapel.rectTransform.anchoredPosition.x speedToPixel(currentSpeed));

        mode = 0;
    }

    // Update is called once per frame
    void Update()
    {
        if (nextActionTime.time > nextActionTime)
        {
            nextActionTime = Time.time + period;
            speedVal.text = Mathf.RoundToInt(currentSpeed).ToString();
            if ((targetSpeed == float.MinValue) || (targetSpeed == currentSpeed && mode == 0))
            {
                targetSpeed = float.MinValue;
                targetVal.text = currentSpeed.ToString();
                hideBug();
            }
            else
            {
                targetVal.text = Mathf.RoundToInt(targetSpeed).ToString();
                bug.rectTransform.anchoredPosition = new Vector3(bug.rectTransform.anchoredPosition.x, bugPosition(targetSpeed));

                if (mode == 0)
                {
                    //Block the other actions
                    global.actionInProgress = true;

                    if (targetSpeed < currentSpeed)
                    {
                        currentSpeed = Mathf.Clamp(currentSpeed - changeSpeed, targetSpeed, float.PositiveInfinity);
                        tapel.rectTransform.anchoredPosition = new Vector3(tapel.rectTransform.anchoredPosition.x, speedToPixel(currentSpeed));
                    }
                    else if (targetSpeed > currentSpeed)
                    {
                        currentSpeed = Mathf.Clamp(currentSpeed + changeSpeed, float.NegativeInfinity, targetSpeed);
                        tapel.rectTransform.anchoredPosition = new Vector3(tapel.rectTransform.anchoredPosition.x, speedToPixel(currentSpeed));
                    }

                    if (targetSpeed == currentSpeed)
                    {
                        //Unblock the other actions
                        global.actionInProgress = false;
                    }
                }
            }
        }
    }
}
