using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class VsTape : MonoBehaviour, IField
{
    //Common across UI
    Global global;
    DataManager dataManager;
    public Material mat;
    public int mode;

    //Local to Vs
    Image pointer;
    public Image bug;
    Image tape;
    Text vsVal;
    Text targetVal;

    float centreHeight = (float)58.5;
    float centerBugHeight = (float)59.1;
    public float currentVs;
    public float targetVs;
    float oldTargetVs;

    float nextActionTime = 0.0f;
    float period = 0.03f;
    float changeSpeed;
    //for direct touch 
    float centerTapeHeight = 61;

    float pixelToVs(float pix)
    {
        return (float)((pix - centerHeight) / 0.09);
    }

    float vsToPixel(float vs)
    {
        return (float)(centreHeight + (vs * 0.09));
    }

    public float bugPosition(float vs)
    {
        return (float)(centerBugHeight + (vs * 0.09));
    }

    void hideBug()
    {
        bug.rectTransform.anchoredPosition = new Vector3(bug.rectTransform.anchoredPosition.x, -10000);
    }

    public float tapePosition(float vs)
    {
        return (float)(centerTapeHeight + (vs * 0.09));
    }

    public int toggleMode()
    {
        if (mode == 0)
        {
            if (global.target != 2)
            {
                ++dataManager.wrongFieldSelected;
                global.playSound(1);
                StartCoroutine(global.failure(2));
                return -1;
            }

            dataManager.enterPhase0Data();
            mode = 1;
            //Chance the visual mode
            global.changeUiMode("Canvas/UI/Vs/Vs Target Box", "Canvas/UI/Vs/Vs Target Box/Vs Target Value", mode);
            oldTargetVs = targetVs;
        }
        else
        {
            //Make a change in 1.5 seconds
            changeSpeed = Mathf.Abs(targetVal - currentVs) * 20 / (1.5f / Time.deltaTime);
            mode = 0;
            global.changeUiMode("Canvas/UI/Vs/Vs Target Box", "Canvas/UI/Vs/Vs Target Box/Vs Target Value", mode);
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
        targetVs = Mathf.Clamp(value, -2000, 2000);

        if (overshoot)
        {
            dataManager.enterOvershootValue(targetVs);
        }
    }


    //0: Failure
    //1: Success
    public int confirmTarget()
    {
        if (targetVs != global.targetVs)
        {
            global.playSound(1);
            return 0;
        }
        else
        {
            global.targetVsReached = true;
            global.playSound(0);
            return 1;
        }
    }

    public void restoreOldTarget()
    {
        targetVs = oldTargetVs;
    }

    public float getCurrentValue()
    {
        return currentVs;
    }

    public int getMode()
    {
        return mode;
    }

    // Start is called before the first frame update
    void Start()
    {
        pointer = GameObject.Find("Canvas/UI/Vs/Vs Pointer").GetComponent<Image>();
        vsVal = GameObject.Find("Canvas/UI/Vs/Vs Pointer/Vs Value").GetComponent<Text>();
        targetVal = GameObject.Find("Canvas/UI/Vs/Vs Target Box/Vs Target Value").GetComponent<Text>();
        global = GameObject.Find("Global").GetComponent<Global>();
        dataManager = GameObject.Find("Global").GetComponent<DataManager>();

        currentVs = 0;
        targetVs = float.MinValue;

        pointer.rectTransform.anchoredPosition = new Vector3(pointer.rectTransform.anchoredPosition.x, pixelToVs(currentVs));
        mode = 0;

        if (SceneManager.GetActiveScene().name == "TouchDirect")
        {
            tape = GameObject.Find("Canvas/UI/Vs/Moving").GetComponent<Image>();
            bug = GameObject.Find("Canvas/UI/Vs/Moving/Vs Bug").GetComponent<Image>();
            centerBugHeight = (float)-1.9;
        }
        else
        {
            bug = GameObject.Find("Canvas/UI/Vs/Moving/Vs Bug").GetComponent<Image>();
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (Time.time > nextActionTime)
        {
            nextActionTime = Time.time + period;
            //Display multiple of 20
            // https://stackoverflow.com/questions/15154457/rounding-integers-to-nearest-multiple-of-10
            int rem = Mathf.RoundToInt(currentVs) % 20;
            vsVal.text = (rem >= 10 ? (Mathf.RoundToInt(currentVs) - rem + 20) : Mathf.RoundToInt(currentVs) - rem).ToString();

            if ((targetVs = float.MinValue) || ((targetVs == currentVs) && mode == 0))
            {
                targetVs = float.MinValue;
                targetVal.text = 0.ToString();
                hideBug();

                //Airplane is stable uphill or downhill completed
                currentVs = 0;
                pointer.rectTransform.anchoredPosition = new Vector3(pointer.rectTransform.anchoredPosition.x, vsToPixel(currentVs));
            }
            else
            {
                //Display multiple of 100
                // https://stackoverflow.com/questions/15154457/rounding-integers-to-nearest-multiple-of-10
                int rem2 = Mathf.RoundToInt(targetVs) % 100;
                targetVal.text = (rem2 >= 50 ? (Mathf.RoundToInt(targetVs) - rem2 + 100) : Mathf.RoundToInt(targetVs) - rem2).ToString();

                bug.rectTransform.anchoredPosition = new Vector3(bug.rectTransform.anchoredPosition.x, bugPosition(targetVs));

                if (mode == 0)
                {
                    //block other actions
                    global.actionInProgress = true;

                    if (targetVs < currentVs)
                    {
                        currentVs = Mathf.Clamp(currentVs - changeSpeed, targetVs, float.PositiveInfinite);
                        pointer.rectTransform.anchoredPosition = new Vector3(pointer.rectTransform.anchoredPosition.x, vsToPixel(currentVs));
                    }
                    else if (targetVs > currentVs)
                    {
                        currentVs = Mathf.Clamp(currentVs + changeSpeed, float.NegativeInfinity, targetVs);
                        pointer.rectTransform.anchoredPosition = new Vector3(pointer.rectTransform.anchoredPosition.x, vsToPixel(currentVs));
                    }
                    else
                    {
                        //Unblock other actions
                        global.actionInProgress = false;
                    }
                }
            }
        }
    }
}
