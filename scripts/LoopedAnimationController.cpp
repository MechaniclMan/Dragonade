#include "General.h"
#include "LoopedAnimationController.h"
#include "ScriptableGameObj.h"
#include "engine_phys.h"
#include "PhysicalGameObj.h"

// -------------------------------------------------------------------------------------------------

LoopedAnimationController::LoopedAnimationController ( GameObject* pObj )
{
  m_objID                 = 0;    // Will be set in the Attach callback
  m_animationName         = NULL;

  if ( pObj )
    pObj->Add_Observer(this);
}

// -------------------------------------------------------------------------------------------------

LoopedAnimationController::~LoopedAnimationController()
{
  if ( m_objID != NULL )
  {
    if ( GameObject* pObj = Commands->Find_Object(m_objID) )
      pObj->Remove_Observer(this);
  }

  delete [] m_animationName;
}

// -------------------------------------------------------------------------------------------------

void LoopedAnimationController::PlayAnimation ( const char* animation, int startFrame, int endFrame, int nRepetitions )
{
  if ( m_objID != NULL )
  {
    if ( GameObject* pObj = Commands->Find_Object(m_objID) )
    {
      delete [] m_animationName;
      m_animationName = new char[strlen(animation)+1];
      memcpy ( m_animationName, animation, strlen(animation)+1 );

      // Store animation loop details
      m_startFrame   = startFrame;
      m_endFrame     = endFrame;
      m_nRepetitions = nRepetitions;

      // If we are playing the same animation that was previously playing on the target object we
      // have to clear the animation first or things don't work properly for... some... reason..?
      if (Get_Animation_Name(pObj) && (_stricmp(animation, Get_Animation_Name(pObj)) == 0))
        pObj->As_PhysicalGameObj()->Clear_Animation();

      // Start the animation
      Commands->Set_Animation ( pObj, m_animationName, false, NULL, (float)m_startFrame, (float)m_endFrame, false );
    }
  }
}

// -------------------------------------------------------------------------------------------------

void LoopedAnimationController::StopAnimation()
{
  if ( m_objID != NULL )
  {
    if ( GameObject* pObj = Commands->Find_Object(m_objID) )
    {
      if ( m_animationName != NULL && Get_Animation_Name(pObj) && _stricmp(m_animationName, Get_Animation_Name(pObj)) == 0 )
        pObj->As_PhysicalGameObj()->Clear_Animation();

      delete [] m_animationName;
      m_animationName = NULL;
    }
  }
}

// -------------------------------------------------------------------------------------------------

void LoopedAnimationController::Attach ( GameObject* pObj )
{
  m_objID = Commands->Get_ID(pObj);
}

// -------------------------------------------------------------------------------------------------

void LoopedAnimationController::Detach ( GameObject* pObj )
{
  m_objID = NULL;
}

// -------------------------------------------------------------------------------------------------

void LoopedAnimationController::Animation_Complete ( GameObject *pObj, const char *animation_name )
{
  if ( m_animationName != NULL && _stricmp(m_animationName, animation_name) == 0 )
  {
    // Check if we have ran out of repetitions - if so don't restart the animation and reset data
    if ( m_nRepetitions != 0 && --m_nRepetitions == 0 )
    {
      delete [] m_animationName;
      m_animationName = NULL;
      return;
    }

    // OK, go ahead and restart the animation
    pObj->As_PhysicalGameObj()->Clear_Animation();
    Commands->Set_Animation ( pObj, m_animationName, false, NULL, (float)m_startFrame, (float)m_endFrame, false );
  }
}

// -------------------------------------------------------------------------------------------------