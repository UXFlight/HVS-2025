using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class DoubleTaskV5 : MonoBehaviour
{
    Image cross;
    Image box;
    Image bg;
    Image pitch;

    DataManager dataManager;

    public float distValue;

    int invertedYAxis = 1;

    float nextActionTime = 0.0f;
    public float period = 0.03f;

    int nextPosH;
    int nextPosV;

    float bufferH;
    float bufferV;
    float maxBuffer = 10f;
    float minBuffer = 3f;

    float purpSpeedH;
    float purpSpeedV;
    float maxPurpSpeed = (float)0.5;
    float minPurpSpeed = (float)0.25;

    float yelSpeedH = (float)1.5;
    float yelSpeedV = (float)1.5;

    void getNextH()
    {
        nextPosH = Random.Range(-109, 85);
        bufferH = Time.time + Random.Range(minBuffer, maxBuffer);
        purpSpeedH = Random.Range(minPurpSpeed, maxPurpSpeed);
    }

    void getNextV()
    {
        nextPosV = Random.Range(-42, 195);
        bufferV = Time.time + Random.Range(minBuffer, maxBuffer);
        purpSpeedV = Random.Range(minPurpSpeed, maxPurpSpeed);
    }

    void updateBg()
    {
        bg.rectTransform.eulerAngles = new Vector3(0, 0, (float)(-0.056 * cross.rectTransform.anchoredPosition.x + -0.618));
        pitch.rectTransform.anchoredPosition = new Vector3(0, (float) 0.25 * (cross.rectTransform.anchoredPosition.y - 82), 0);
    }

    // Start is called before the first frame update
    void Start()
    {
        //Link variables and enter values 
        cross = GameObject.Find("Canvas/DoubleTask/Tracking Cross").GetComponent<Image>();
        box = GameObject.Find("Canvas/DoubleTask/Target Box").GetComponent<Image>();
        bg = GameObject.Find("Canvas/Background/bg").GetComponent<Image>();
        //pitchLimiter = GameObject.Find("Canvas/Background/bg/Pitch Limiter").GetComponent<>();
        // need to rotate pitchLimiter with pitch to prevent it from hiding it during rotation
        pitch = GameObject.Find("Canvas/Background/bg/Pitch Limiter/Pitch").GetComponent<Image>();

        dataManager = GameObject.Find("Global").GetComponent<DataManager>();

        getNextH();
        getNextV();
    }

    // Update is called once per frame
    void Update()
    {
        if (Time.time > nextActionTime)
        {
            nextActionTime = Time.time + period;

            //Horizontal
            if (Time.time > bufferH)
            {
                float currentPosH = box.rectTransform.anchoredPosition.x;
                float currentPosV = box.rectTransform.anchoredPosition.y;
                if (nextPosH > currentPosH)
                {
                    box.rectTransform.anchoredPosition = new Vector3(currentPosH + purpSpeedH, currentPosV);
                    if (box.rectTransform.anchoredPosition.x > nextPosH)
                    {
                        box.rectTransform.anchoredPosition = new Vector3(nextPosH, currentPosV);
                    }
                }
                else if (nextPosH < currentPosH)
                {
                    box.rectTransform.anchoredPosition = new Vector3(currentPosH - purpSpeedH, currentPosV);
                    if (box.rectTransform.anchoredPosition.x < nextPosH)
                    {
                        box.rectTransform.anchoredPosition = new Vector3(nextPosH, currentPosV);
                    }
                }
                else
                {
                    getNextH();
                }
            }

            //Vertical
            if (Time.time > bufferV)
            {
                float currentPosH = box.rectTransform.anchoredPosition.x;
                float currentPosV = box.rectTransform.anchoredPosition.y;
                if (nextPosV > currentPosV)
                {
                    box.rectTransform.anchoredPosition = new Vector3(currentPosH, currentPosV + purpSpeedV);
                    if (box.rectTransform.anchoredPosition.y > nextPosV)
                    {
                        box.rectTransform.anchoredPosition = new Vector3(currentPosH, nextPosV);
                    }
                }
                else if (nextPosV < currentPosV)
                {
                    box.rectTransform.anchoredPosition = new Vector3(currentPosH, currentPosV - purpSpeedV);
                    if (box.rectTransform.anchoredPosition.y < nextPosV)
                    {
                        box.rectTransform.anchoredPosition = new Vector3(currentPosH, nextPosV);
                    }
                }
                else
                {
                    getNextV();
                }
            }

            float signX = Mathf.Sign(Input.GetAxis("Horizontal"));
            float signY = Mathf.Sign(Input.GetAxis("Vertical"));

            
            //Debug.Log()

            //Displacement of the yellow FD with the joystick
            cross.rectTransform.anchoredPosition = new Vector3(
                Mathf.Clamp(cross.rectTransform.anchoredPosition.x + (Mathf.Pow(Mathf.Abs(Input.GetAxis("Horizontal")), 2) * yelSpeedH * signX), -109, 85),
                Mathf.Clamp(cross.rectTransform.anchoredPosition.y + invertedYAxis * (Mathf.Pow(Mathf.Abs(Input.GetAxis("Vertical")), 2) * yelSpeedV * signY), -42, 195));
            updateBg();
            
            //Display of the distance
            distValue = (Mathf.Sqrt(Mathf.Pow(cross.rectTransform.anchoredPosition.y - box.rectTransform.anchoredPosition.y, 2) +
                Mathf.Pow(cross.rectTransform.anchoredPosition.x - box.rectTransform.anchoredPosition.x, 2)) / 50);

            dataManager.enterDoubleTaskDistance(distValue);
        }
    }
}
