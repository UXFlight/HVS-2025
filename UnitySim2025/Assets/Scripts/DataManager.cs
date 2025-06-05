using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using System.IO;

public class DataManager : MonoBehaviour
{
    public StreamWriter writer;
    Global global;
    public int participant;
    public int device;
    /* switch to storing devices in a array as scenes will have multiple devices
    public List<int> devices = new List<int>();
    */

    // possible values for val
    //0: small hdg
    //1: med hdg
    //2: large hdg
    //3: small baro
    //4: med baro
    //5: large baro
    //6: small alt
    //7: med alt
    //8: large alt
    //9: small vs
    //10: med vs
    //11: large vs
    //12: small ias
    //13: med ias
    //14: large ias
    // switch to using 'enum task' in Enums.cs latter on
    public int val;

    //Phase 0 (lecture + navigation)
    public int wrongFieldSelected; // counter
    public float timeNavigating;
    public float doubleTaskNavigating;

    //Phase 1 (between the right mode and value)
    public int nbValError;
    public int nbInteractionVal;
    public int nbOvershootVal;
    public float avgOvershootAmplitudeVal;
    public float timeToVal;
    public float doubleTaskAvgToVal;

    //The number of data of double spot to be aggregated and the sum of all data
    int doubleTaskNb;
    float doubleTaskDist;

    //Timers
    float timerPhase0;
    float timerPhase1;

    //Four managing overshoots
    float lastValue;
    float positionToTarget; // -1, 0, 1
    float lastPositionToTarget; // -1, 0, 1
    public float overshootInitialPosition; // -1, 1
    public float target;
    float max;
    float min;
    bool initialRun = true; //Before passing the target once
    List<float> over;
    List<float> under;

    float lastZone;
    float lastLastZone; // not needed?

    public void enterOvershootValue(float value)
    {
        // if it's an Hdg target
        if (global.target == 4)
        {
            if (value == target)
            {
                positionToTarget = 0;
            }
            else if (over.Contains(value))
            {
                positionToTarget = 1;
            }
            else if (under.Contains(value))
            {
                positionToTarget = -1;
            }
            else
            {
                positionToTarget = -2;
            }
            // refactor to reduce length?

            if (positionToTarget != lastPositionToTarget)
            {
                lastLastZone = lastPositionToTarget;
            }

            if (positionToTarget != 0)
            {
                if (lastLastZone != -2)
                {
                    if (positionToTarget == -1 && min != 0)
                    {
                        ++nbOvershootVal;
                        avgOvershootAmplitudeVal = min;
                        //Debug.Log("1, min: " + min);
                        min = 0;
                    }

                    if (over.IndexOf(value) + 1 > max)
                    {
                        max = over.IndexOf(value) + 1; // because index starts at 0 which represents overshoot of 1
                        //Debug.Log("Max: " + max);
                    }
                }

                if (positionToTarget == -1)
                {
                    if (lastPositionToTarget == 1 && max != 0)
                    {
                        ++nbOvershootVal;
                        avgOvershootAmplitudeVal = max;
                        //Debug.Log("2, max: " + max);
                        max = 0;
                    }

                    if (under.IndexOf(value) + 1 > min)
                    {
                        min = under.IndexOf(value) + 1;
                        //Debug.Log("Min: " + min);
                    }
                }
            }
            else
            {
                if (lastPositionToTarget == -1 && min != 0) //if you have passed from below and you are not in the first pass (min != 0)
                {
                    ++nbOvershootVal;
                    avgOvershootAmplitudeVal += min;
                    //Debug.Log("3, min: " + min);
                    min = 0; //reset the min
                }

                else if (lastPositionToTarget == 1 && max != 0) //if you have passed from above and you are not in the first pass (max != 0)
                {
                    ++nbOvershootVal;
                    avgOvershootAmplitudeVal += max;
                    //Debug.Log("4, max: " + max);
                    max = 0; //reset the max
                }
            }
        }
        else //if not Hdg
        {
            positionToTarget = value < target ? -1 : (value > target ? 1 : 0);

            if (positionToTarget != 0) //is an overshoot
            {
                if (!(initialRun && (positionToTarget == overshootInitialPosition))) //si on a passé le target
                {
                    if (positionToTarget == 1) //we exceeded the target from above
                    {
                        if (lastPositionToTarget == -1 && !initialRun) //passed directly from -1 to 1
                        {
                            ++nbOvershootVal;
                            avgOvershootAmplitudeVal += target - min;
                            min = target; //reset min
                        }

                        if (value > max)
                        {
                            max = value;
                        }
                    }
                    else if (positionToTarget == -1) //we exceeded the target from below
                    {
                        if (lastPositionToTarget == 1 && !initialRun) //passed directly from 1 to -1
                        {
                            ++nbOvershootVal;
                            avgOvershootAmplitudeVal += max - target;
                            max = target; //reset the max
                        }

                        if (value < min)
                        {
                            min = value;
                        }
                    }

                    initialRun = false; //we passed the target once
                }
            }
            else //position == 0
            {
                if (lastPositionToTarget == -1 && !initialRun)
                {
                    ++nbOvershootVal;
                    avgOvershootAmplitudeVal += target - min;
                    min = target; //reset the min
                }

                else if (lastPositionToTarget == 1 && !initialRun)
                {
                    ++nbOvershootVal;
                    avgOvershootAmplitudeVal += max - target;
                    max = target; //reset the max
                }
            }
        }
        lastPositionToTarget = positionToTarget;
        lastValue = value;
    }

    public void setOvershootArrays()
    {
        //if it's a Hdg target
        if (global.target == 4)
        {
            over = new List<float>();
            for (int i = 1; i < 45; i++)
            {
                float newVal = (target + i) % 360;

                if (newVal == 0)
                {
                    newVal = 360;
                }

                over.Add(newVal);
            }

            under = new List<float>();
            for (int i = 1; i < 45; i++)
            {
                float newVal = target - i;

                if (newVal == 0)
                {
                    newVal = 360;
                }
                else if (newVal < 0)
                {
                    newVal = 360 + newVal;
                }

                under.Add(newVal);
            }
        }
    }

    public void enterDoubleTaskDistance(float dist)
    {
        ++doubleTaskNb;
        doubleTaskDist += dist;
    }

    public void startTimerPhase0()
    {
        timerPhase0 = Time.time;
    }

    public void enterPhase0Data()
    {
        //Write and then reset the double-touch data
        doubleTaskNavigating = doubleTaskDist / doubleTaskNb;
        doubleTaskDist = 0;
        doubleTaskNb = 0;

        timeNavigating = Time.time - timerPhase0;

        timerPhase1 = Time.time;

        //Reset the extremes to overshoot (here to make sure they are well defined)
        if (global.target == 4)
        {
            min = 0;
            max = 0;
        }
        else
        {
            min = target;
            max = target;
        }
        setOvershootArrays();
    }

    public void enterPhase1Data()
    {
        doubleTaskAvgToVal = doubleTaskDist / doubleTaskNb;

        if (nbOvershootVal != 0) // to avoid division by 0
        {
            avgOvershootAmplitudeVal = avgOvershootAmplitudeVal / nbOvershootVal;
        }

        timeToVal = Time.time - timerPhase1;
    }

    // change how this gets device name
    // iterate through "Canvas/Scene Devices" and get each device associate
    //      with respecitve enum, append these to devices (not yet implemented)
    void getDevice()
    {
        string deviceName = SceneManager.GetActiveScene().name;

        switch (deviceName)
        {
            case "Joystick":
                device = 0;
                break;
            case "RotaryEncoder":
                device = 1;
                break;
            case "RotaryJoystick":
                device = 2;
                break;
            case "TouchColocKeyboard":
                device = 3;
                break;
            case "TouchDirect":
                device = 4;
                break;
            case "TouchGroupedKeyboard":
                device = 5;
                break;
            case "TouchGroupedSlider":
                device = 6;
                break;
        }
    }

    //add feature so that file is created from info settup on a main page
    void openWriter()
    {
        string filePath = $"{Application.dataPath}/Data/Participant/{participant}.csv";

        if (File.Exists(filePath))
        {
            writer = new StreamWriter(filePath, append: true);
            return;
        }

        filePath.Create(filePath).Close();

        //Debug.Log(filepath);
        //This is the writer, it writes to the filepath
        writer = new StreamWriter(filePath);
        writer.WriteLine("participant,device,val," +
                "wrongFieldSelected,timeNavigating,doubleTaskNavigating," +
                "nbValError,nbInteractionVal,nbOvershootVal,avgOvershootAmplitudeVal," +
                "timeToVal,doubleTaskAvgToVal");
    }

    public void closeWriter()
    {
        writer.close();
    }

    void resetVariables()
    {
        val = 0; //not necessary

        //Phase 0 (before the correct mode)
        wrongFieldSelected = 0;
        timeNavigating = 0;
        doubleTaskNavigating = 0;

        //Phase 1 (between the correct mode and the 1st value, or the 2nd and 1st value)
        nbValError = 0;
        nbInteractionVal = 0;
        nbOvershootVal = 0;
        avgOvershootAmplitudeVal = 0;
        timeToVal = 0;
        doubleTaskAvgToVal = 0;

        doubleTaskNb = 0;
        doubleTaskDist = 0;

        timerPhase0 = 0;
        timerPhase1 = 0;

        lastPositionToTarget = -10;
        initialRun = true;
    }

    public void writeTask()
    {
        writer.WriteLine(participant.ToString() +
            "," + device.ToString() +
            "," + val.ToString() +
            "," + wrongFieldSelected.ToString() +
            "," + timeNavigating.ToString().Replace(',', '.') +
            "," + doubleTaskNavigating.ToString().Replace(',', '.') +
            "," + nbValError.ToString() +
            "," + nbInteractionVal.ToString() +
            "," + nbOvershootVal.ToString() +
            "," + avgOvershootAmplitudeVal.ToString("n2").Replace(',', '.') +
            "," + timeToVal.ToString().Replace(',', '.') +
            "," + doubleTaskAvgToVal.ToString().Replace(',', '.'));

        resetVariables();
        writer.Flush();
    }

    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        global = GameObject.Find("Global").GetComponent<Global>();
        getDevice();
        openWriter();
        resetVariables();
        //writeTask();
        //closeWriter();
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
