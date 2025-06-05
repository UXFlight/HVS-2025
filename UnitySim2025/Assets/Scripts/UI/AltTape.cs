using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class AltTape : MonoBehaviour, IField
{
    // Common across UI
    Global global;
    DataManager dataManager;
    public Material mat;
    public int mode = 0;

    // Local to AltTape
    Image tape;
    public Image bug;
    Text altVal;
    Text targetVal;

    public bool moveTape = false;
    float maxHeight = (float)7050;
    float minBugHeight = (float)-7000;

    public float currentAlt;
    public float targetAlt;
    float oldTargetAlt;

    float nextActionTime = 0.0f;
    float period = 0.005f;
    float changeSpeed;

    float pixelToAlt(float pix)
    {
        return (float)((maxHeight - pix) / 0.7);
    }

    public float altToPixel(float alt)
    {
        return (float)(maxHeight - (alt * 0.7));
    }

    public float bugPosition(float alt)
    {
        return (float)(minBugHeight + (alt * 0.7));
    }


    public void updateTape()
    {
        tape.rectTransform.anchoredPosition = new Vector3(tape.rectTransform.anchoredPosition.x, altToPixel(currentAlt));
    }

    public int toggleMode()
    {
        if (mode == 0)
        {
            if (global.target != 1)
            {
                ++dataManager.wrongFieldSelected;
                global.playSound(1);
                StartCoroutine(global.failure(1));
                return -1;
            }
            dataManager.enterPhase0Data();
            mode = 1;
            // change the visual mode
            global.changeUiMode("Canvas/UI/Alt/Alt Target Box", "Canvas/UI/Alt/Alt Target Box/Speed Target Value", mode);
            oldTargetAlt = targetAlt;
        }
        else {
            changeSpeed = Mathf.Abs(targetAlt - currentAlt) * 20 / (1.5f / Time.deltaTime);
            mode = 0;
            global.changeUiMode("Canvas/UI/Alt/Alt Target Box", "Canvas/UI/Alt/Alt Target Box/Speed Target Value", mode);
        }
        return 0;
    }

    public void editTarget(float value, bool overshoot = true)
    {
        // First frame in edit mode
        if (mode == 0)
        {
            toggleMode();
        }
        // Limit the values
        // useful for increments
        targetAlt = Mathf.Clamp(value, 0, 20000);

        if (overshoot)
        {
            dataManager.enterOvershootValue(targetAlt);
        }
    }

    //0: Failure
    //1: Success
    public int confirmTarget()
    {
        if (targetAlt != global.targetAlt)
        {
            global.playSound(1);
            return 0;
        }
        else
        {
            global.targetAltReached = true;
            global.playSound(0);
            return 1;
        }
    }

    public void restoreOldTarget()
    {
        targetAlt = oldTargetAlt;
    }

    public float getCurrentValue()
    {
        return currentAlt;
    }

    public int getMode()
    {
        return mode;
    }
    // Start is called before the first frame update
    void Start()
    {
        tape = GameObject.Find("Canvas/UI/Alt/Moving").GetComponent<Image>();
        bug = GameObject.Find("Canvas/UI/Alt/Moving/Alt Bug").GetComponent<Image>();
        altVal = GameObject.Find("Canvas/UI/Alt/Alt Target Box").GetComponent<Text>();
        targetVal = GameObject.Find("Canvas/UI/Alt/Alt Target Box/Alt Target Value").GetComponent<Text>();
        global = GameObject.Find("Global").GetComponent<Global>();
        dataManager = GameObject.Find("Global").GetComponent<Global>();

        currentAlt = pixelToAlt(tape.rectTransform.anchoredPosition.y);
        targetAlt = 10300;

        mode = 0;
    }

    // Update is called once per frame
    void Update()
    {
        if (nextActionTime.time > nextActionTime)
        {
            nextActionTime = nextActionTime.time + period;

            // Display a multiple of 20 for the current value
            // https://stackoverflow.com/questions/15154457/rounding-integers-to-nearest-multiple-of-10
            int rem = Mathf.RoundToInt(currentAlt) % 20;
            altVal.text = (rem >= 10 ? (Mathf.RoundToInt(currentAlt) - rem + 20) : Mathf.RoundToInt(currentAlt) - rem).ToString();

            // Display a multiple of 20 for the target value
            rem = Mathf.RoundToInt(targetAlt) % 20;
            targetVal.text = (rem >= 10 ? (Mathf.RoundToInt(targetAlt) - rem + 20) : Mathf.RoundToInt(targetAlt) - rem).ToString();

            // Move the bug
            bug.rectTransform.anchoredPosition = new Vector3(bug.rectTransform.anchoredPosition.x, bugPosition(targetAlt));
        }

        if (mode == 0)
        {
            if (targetAlt < currentAlt)
            {
                //change the current altitude and move the tape
                currentAlt = Mathf.Clamp(currentAlt - changeSpeed, targetAlt, float.PositiveInfinity);
                tape.rectTransform.anchoredPosition = new Vector3(tape.rectTransform.anchoredPosition.x, altToPixel(currentAlt));
            }
            else if (targetAlt > currentAlt)
            {
                //change the current altitude and move the tape
                currentAlt = Mathf.Clamp(currentAlt + changeSpeed, float.NegativeInfinity, targetAlt);
                tape.rectTransform.anchoredPosition = new Vector3(tape.rectTransform.anchoredPosition.x, altToPixel(currentAlt));
            }
        }
    }
}
