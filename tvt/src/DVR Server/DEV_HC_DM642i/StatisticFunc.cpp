/**CPP*************************************************************************
 File            : StatisticFunc.cpp
 Subsystem       : 
 Function Name(s): CStatisticFunc
 Author          : YiQiu
 Date            : 2010-4-9  
 Time            : 10:06
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
**************************************************************************cpp**/
#include "stdafx.h"
#include "DSP.h"




BOOL CDSP::AddStatistic(
    int nChannelID,
    int nDeviceID,
    const WPG_Rule& Rule )
{
    StatisticSetting* p = m_pStatisticRule[nDeviceID];
    if ( p == NULL )
    {
        ASSERT(false);
        TRACE("CDSP::AddStatistic Failed!\n");
        return FALSE;
    }

    if ( p->bIsEnable )
    {
        ASSERT(false);
        TRACE("CDSP::AddStatistic Is Already Start!\n");
        return FALSE;
    }

    BOOL bRc = _AddStatistic(nChannelID, Rule);
    if ( bRc )
    {
        ASSERT(false);
        p->Rule = Rule;
        p->bIsEnable = TRUE;
    }

    return bRc;
}

BOOL CDSP::RemoveStatistic( 
    int nChannelID, 
    int nDeviceID,
    const IV_RuleID& RuleID )
{
    StatisticSetting* p = m_pStatisticRule[nDeviceID];
    if ( p == NULL || 
         0 == memcmp(&RuleID, p->Rule.ruleId, 16) )
    {
        ASSERT(false);
        TRACE("CDSP::AddStatistic Failed!\n");
        return FALSE;
    }

    if ( !p->bIsEnable )
    {
        ASSERT(false);
        TRACE("CDSP::RemoveStatistic Is Already Stop!\n");
        return FALSE;
    }

    BOOL bRc = _RemoveStatistic(nChannelID, p->Rule);
    if ( bRc )
    {
        ASSERT(false);
        p->bIsEnable = FALSE;
    }
    
    return bRc;
}

BOOL CDSP::EnableStatistic( 
    int nChannelID,
    int nDeviceID, 
    const IV_RuleID& RuleID,
    BOOL bEnable )
{
    StatisticSetting* p = m_pStatisticRule[nDeviceID];
    if ( p == NULL || 
         0 == memcmp(&RuleID, p->Rule.ruleId, 16) )
    {
        ASSERT(false);
        TRACE("CDSP::EnableStatistic Failed!\n");
        return FALSE;
    }

    if ( !p->bIsEnable )
    {
        ASSERT(false);
        TRACE("CDSP::EnableStatistic Rule is Not Set!\n");
        return FALSE;
    }
    
    WPG_Rule& Rule = p->Rule;
    Rule.isEnabled = bEnable;
    WPG_LINE_CROSS_DIRECTION dir = 
        Rule.ruleDescription.description.tripwireEventDescription.direction;
    if ( dir == INVALID_DIRECTION )
    {
        TRACE("CDSP::EnableStatistic Invalid Rule!\n");
        return FALSE;
    }

    if ( dir == ANY_DIRECTION ||
         dir == LEFT_TO_RIGHT )
    {
        SetIVSpecialParam(
            PT_PCI_SET_UPDATE_RULE, nChannelID, 0, Rule);
    }
    if ( dir == ANY_DIRECTION ||
         dir == RIGHT_TO_LEFT )
    {
        IV_RuleID& Sencond = (IV_RuleID&)Rule.ruleId;
        ++Sencond.RuleID.ID;
        SetIVSpecialParam(
            PT_PCI_SET_UPDATE_RULE, nChannelID, 0, Rule);
        --Sencond.RuleID.ID;
    }
    return TRUE;
}

BOOL CDSP::ModifyStatistic( 
    int nChannelID,
    int nDeviceID,
    const WPG_Rule& Rule )
{
    StatisticSetting* p = m_pStatisticRule[nDeviceID];
    if ( p == NULL || 
         0 == memcmp(Rule.ruleId, p->Rule.ruleId, 16) )
    {
        ASSERT(false);
        TRACE("CDSP::ModifyStatistic Failed!\n");
        return FALSE;
    }

    if ( !p->bIsEnable )
    {
        ASSERT(false);
        TRACE("CDSP::ModifyStatistic Rule is Not Set!\n");
        return FALSE;
    }

    p->Rule = Rule;
    WPG_LINE_CROSS_DIRECTION dir = 
        Rule.ruleDescription.description.tripwireEventDescription.direction;
    if ( dir == INVALID_DIRECTION )
    {
        TRACE("CDSP::ModifyStatistic Invalid Rule!\n");
        return FALSE;
    }

    if ( dir == ANY_DIRECTION ||
         dir == LEFT_TO_RIGHT )
    {
        SetIVSpecialParam(
            PT_PCI_SET_UPDATE_RULE, nChannelID, 0, Rule);
    }
    if ( dir == ANY_DIRECTION ||
         dir == RIGHT_TO_LEFT )
    {
        IV_RuleID& Sencond = (IV_RuleID&)Rule.ruleId;
        ++Sencond.RuleID.ID;
        SetIVSpecialParam(
            PT_PCI_SET_UPDATE_RULE, nChannelID, 0, Rule);
        --Sencond.RuleID.ID;
    }
    return TRUE;
}

BOOL CDSP::_AddStatistic(
    int nChannelID,
    const WPG_Rule& Rule )
{
    WPG_LINE_CROSS_DIRECTION dir = 
        Rule.ruleDescription.description.tripwireEventDescription.direction;
    if ( dir == INVALID_DIRECTION )
    {
        TRACE("CDSP::_AddStatistic Invalid Rule!\n");
        return FALSE;
    }

    if ( dir == ANY_DIRECTION ||
         dir == LEFT_TO_RIGHT )
    {
        SetIVSpecialParam(
            PT_PCI_SET_ADD_RULE, nChannelID, 0, Rule);
    }
    if ( dir == ANY_DIRECTION ||
         dir == RIGHT_TO_LEFT )
    {
        IV_RuleID& Sencond = (IV_RuleID&)Rule.ruleId;
        ++Sencond.RuleID.ID;
        SetIVSpecialParam(
            PT_PCI_SET_ADD_RULE, nChannelID, 0, Rule);
        --Sencond.RuleID.ID;
    }
    return TRUE;
}

BOOL CDSP::_RemoveStatistic(
    int nChannelID,
    const WPG_Rule& Rule )
{
    WPG_LINE_CROSS_DIRECTION dir = 
        Rule.ruleDescription.description.tripwireEventDescription.direction;
    if ( dir == INVALID_DIRECTION )
    {
        TRACE("CDSP::_RemoveStatistic Invalid Rule!\n");
        return FALSE;
    }

    if ( dir == ANY_DIRECTION ||
         dir == LEFT_TO_RIGHT )
    {
        SetIVSpecialParam(
            PT_PCI_SET_DEL_RULE, nChannelID, 0, Rule);
    }
    if ( dir == ANY_DIRECTION ||
         dir == RIGHT_TO_LEFT )
    {
        IV_RuleID& Sencond = (IV_RuleID&)Rule.ruleId;
        ++Sencond.RuleID.ID;
        SetIVSpecialParam(
            PT_PCI_SET_DEL_RULE, nChannelID, 0, Rule);
        --Sencond.RuleID.ID;
    }
    return TRUE;
}



// End of file