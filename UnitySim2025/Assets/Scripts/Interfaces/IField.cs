using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public interface IField
{
    /// <summary>
    /// 
    /// </summary>
    /// <param name="value"></param>
    /// <param name="overshoot"></param>
    public void editTarget(float value, bool overshoot = true);
    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    public int toggleMode();
    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    public int confirmTarget();
    /// <summary>
    /// 
    /// </summary>
    public void restoreOldTarget();
    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    public float getCurrentValue();
    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    public int getMode();
}