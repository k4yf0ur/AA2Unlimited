#include "Actions.h"

#include "Thread.h"

#include "Files\Logger.h"
#include "Functions\AAPlay\Globals.h"

namespace Shared {
namespace Triggers {


void Thread::ShouldNotBeImplemented(std::vector<Value>& params) {
	MessageBox(NULL,TEXT("This message should not have been executed."),TEXT("Error"),0);
}


//int offset, bool condition
void Thread::ConditionalJump(std::vector<Value>& params) {
	int offset = params[0].iVal;
	bool cond = params[1].bVal;
	if (!cond) return;
	this->ip += offset;
}

//void
void Thread::EndExecution(std::vector<Value>& params) {
	this->execFinished = true;
}

//void
void Thread::ConditionalEndExecution(std::vector<Value>& params) {
	if(params[0].bVal) {
		this->execFinished = true;
	}
}

//int card, int roomId
void Thread::NpcMoveRoom(std::vector<Value>& params) {
	int cardSeat = params[0].iVal;
	int roomId = params[1].iVal;
	CharInstData* card = &AAPlay::g_characters[cardSeat];
	if (!card->IsValid()) return;

	card->m_forceAction.conversationId = 0;
	card->m_forceAction.movementType = 2;
	card->m_forceAction.roomTarget = roomId;
	card->m_forceAction.target1 = NULL;
	card->m_forceAction.target2 = NULL;
	card->m_forceAction.unknown = -1;
	card->m_forceAction.unknown2 = 1;
}

//int card, int actionId
void Thread::NpcActionNoTarget(std::vector<Value>& params) {
	int cardSeat = params[0].iVal;
	int actionId = params[1].iVal;
	CharInstData* card = &AAPlay::g_characters[cardSeat];
	if (!card->IsValid()) return;

	card->m_forceAction.conversationId = actionId;
	card->m_forceAction.movementType = 3;
	card->m_forceAction.roomTarget = -1;
	card->m_forceAction.target1 = NULL;
	card->m_forceAction.target2 = NULL;
	card->m_forceAction.unknown = -1;
	card->m_forceAction.unknown2 = 1;
}

//int card, int conversationId, int talkTo
void Thread::NpcTalkTo(std::vector<Value>& params) {
	int cardSeat = params[0].iVal;
	int actionId = params[1].iVal;
	int cardTarget = params[2].iVal;
	CharInstData* card = &AAPlay::g_characters[cardSeat];
	if (!card->IsValid()) return;
	CharInstData* target = &AAPlay::g_characters[cardTarget];
	if (!card->IsValid()) return;

	card->m_forceAction.conversationId = actionId;
	card->m_forceAction.movementType = 3;
	card->m_forceAction.roomTarget = -1;
	card->m_forceAction.target1 = target->m_char->GetActivity();
	card->m_forceAction.target2 = NULL;
	card->m_forceAction.unknown = -1;
	card->m_forceAction.unknown2 = 1;
}

//int card, int conversationId, int talkTo, int talkAbout
void Thread::NpcTalkToAbout(std::vector<Value>& params) {
	int cardSeat = params[0].iVal;
	int actionId = params[1].iVal;
	int cardTarget = params[2].iVal;
	int cardAbout = params[3].iVal;
	CharInstData* card = &AAPlay::g_characters[cardSeat];
	if (!card->IsValid()) return;
	CharInstData* target = &AAPlay::g_characters[cardTarget];
	if (!card->IsValid()) return;
	CharInstData* about = &AAPlay::g_characters[cardAbout];
	if (!card->IsValid()) return;

	card->m_forceAction.conversationId = actionId;
	card->m_forceAction.movementType = 3;
	card->m_forceAction.roomTarget = -1;
	card->m_forceAction.target1 = target->m_char->GetActivity();
	card->m_forceAction.target2 = about->m_char->GetActivity();
	card->m_forceAction.unknown = -1;
	card->m_forceAction.unknown2 = 1;
}

//event response

//bool newAnswer
void Thread::SetNpcResponseAnswer(std::vector<Value>& params) {
	if (this->eventData->GetId() != NPC_RESPONSE) return;
	((NpcResponseData*)eventData)->changedResponse = params[0].bVal;
}

//int percent
void Thread::SetNpcResponsePercent(std::vector<Value>& params) {
	if (this->eventData->GetId() != NPC_RESPONSE) return;
	((NpcResponseData*)eventData)->changedChance = params[0].iVal;
}

namespace {
	void SafeAddCardPoints(int nPoints, int pointKind, int iCardFrom, int iCardTowards) {
		if (pointKind < 0 || pointKind > 3) return;
		CharInstData* cardFrom = &AAPlay::g_characters[iCardFrom];
		CharInstData* cardTowards = &AAPlay::g_characters[iCardTowards];
		if (!cardFrom->IsValid()) return;
		if (!cardTowards->IsValid()) return;
		if (cardFrom == cardTowards) return;

		auto* ptrRel = cardFrom->m_char->GetRelations();
		auto* rel = ptrRel->m_start;
		if (ptrRel == NULL) return;
		for (rel; rel != ptrRel->m_end; rel++) {
			if (rel->m_targetSeat == iCardTowards) {
				break;
			}
		}
		if (rel == ptrRel->m_end) return;

		switch (pointKind) {
		case 0:
			rel->m_lovePoints += nPoints;
			break;
		case 1:
			rel->m_likePoints += nPoints;
			break;
		case 2:
			rel->m_dislikePoints += nPoints;
			break;
		case 3:
			rel->m_hatePoints += nPoints;
		default:
			break;
		}

		AAPlay::ApplyRelationshipPoints(cardFrom->m_char,rel);
	}
}

//int nPoints, int cardFrom, int cardTowards
void Thread::AddCardLovePoints(std::vector<Value>& params) {
	int nPoints = params[0].iVal;
	int iCardFrom = params[1].iVal;
	int iCardTowards = params[2].iVal;

	SafeAddCardPoints(nPoints,0,iCardFrom,iCardTowards);
}

//int nPoints, int cardFrom, int cardTowards
void Thread::AddCardLikePoints(std::vector<Value>& params) {
	int nPoints = params[0].iVal;
	int iCardFrom = params[1].iVal;
	int iCardTowards = params[2].iVal;

	SafeAddCardPoints(nPoints,1,iCardFrom,iCardTowards);
}

//int nPoints, int cardFrom, int cardTowards
void Thread::AddCardDislikePoints(std::vector<Value>& params) {
	int nPoints = params[0].iVal;
	int iCardFrom = params[1].iVal;
	int iCardTowards = params[2].iVal;

	SafeAddCardPoints(nPoints,2,iCardFrom,iCardTowards);
}

//int nPoints, int cardFrom, int cardTowards
void Thread::AddCardHatePoints(std::vector<Value>& params) {
	int nPoints = params[0].iVal;
	int iCardFrom = params[1].iVal;
	int iCardTowards = params[2].iVal;

	SafeAddCardPoints(nPoints,3,iCardFrom,iCardTowards);
}

//int nPoints, int pointKind, int cardFrom, int cardTowards
void Thread::AddCardPoints(std::vector<Value>& params) {
	int nPoints = params[0].iVal;
	int pointKind = params[1].iVal;
	int iCardFrom = params[2].iVal;
	int iCardTowards = params[3].iVal;

	if (pointKind < 0 || pointKind > 3) return;
	CharInstData* cardFrom = &AAPlay::g_characters[iCardFrom];
	CharInstData* cardTowards = &AAPlay::g_characters[iCardTowards];
	if (!cardFrom->IsValid()) return;
	if (!cardTowards->IsValid()) return;
	if (cardFrom == cardTowards) return;

	auto* ptrRel = cardFrom->m_char->GetRelations();
	auto* rel = ptrRel->m_start;
	if (ptrRel == NULL) return;
	for(rel; rel != ptrRel->m_end; rel++) {
		if (rel->m_targetSeat == iCardTowards) {
			break;
		}
	}
	if (rel == ptrRel->m_end) return;
	
	switch(pointKind) {
	case 0:
		rel->m_lovePoints += nPoints;
		break;
	case 1:
		rel->m_likePoints += nPoints;
		break;
	case 2:
		rel->m_dislikePoints += nPoints;
		break;
	case 3:
		rel->m_hatePoints += nPoints;
		break;
	default:
		break;
	}
	
	AAPlay::ApplyRelationshipPoints(cardFrom->m_char,rel);
}


//int seat, int newseat
void Thread::SwitchAAUDataSet(std::vector<Value>& params) {
	int seat = params[0].iVal;
	int newset = params[1].iVal;
	if(!AAPlay::g_characters[seat].m_char) {
		LOGPRIO(Logger::Priority::WARN) << "[Trigger] Invalid card target; seat number " << seat << "\r\n";
		return;
	}
	auto& aau = AAPlay::g_characters[seat].m_cardData;
	aau.SwitchActiveAAUDataSet(newset);
}


/* 
 * A list of all action categories
 */

std::wstring g_ActionCategories[ACTIONCAT_N] = {
	TEXT("General"),
	TEXT("Card Modification"),
	TEXT("Flow Control"),
	TEXT("Character Modification"),
	TEXT("Event Response"),
	TEXT("Npc Action")
};


/*
* A list of all actions available.
*/
std::vector<Action> g_Actions = {
	{
		ACTION_SETVAR, ACTIONCAT_GENERAL, TEXT("Set Variable"), TEXT("Set Variable %p = %p"), TEXT("Sets the value of a given variable to a new one"),
		{ TYPE_INVALID, TYPE_INVALID },
		&Thread::ShouldNotBeImplemented
	},
	{
		ACTION_IF, ACTIONCAT_FLOW_CONTROL, TEXT("If"), TEXT("If %p then"),
		TEXT("Executes Actions if Boolean Expression is true. Use with \"Else if\" and \"Else\" actions."),
		{ TYPE_BOOL },
		&Thread::ShouldNotBeImplemented
	},
	{
		ACTION_ELSEIF, ACTIONCAT_FLOW_CONTROL, TEXT("Else If"), TEXT("Else If %p then"),
		TEXT("Executes Actions if the previous If Action was not executed, and the Boolean Expression is true. Use with \"If\" and \"Else\" actions."
		" If this action does not preceed a \"If\" action, it acts as an \"If\" action instead."),
		{ TYPE_BOOL },
		&Thread::ShouldNotBeImplemented
	},
	{
		ACTION_ELSE, ACTIONCAT_FLOW_CONTROL, TEXT("Else"), TEXT("Else"),
		TEXT("Executes Actions if the preceeding \"If\" and \"Else If\" actions were all not Executed. Use with \"If\" and \"Else\" actions."
		" If this action does not preceed a \"If\" or \"Else if\" action, it is always executed."),
		{ },
		&Thread::ShouldNotBeImplemented
	},
	{
	ACTION_CONDJUMP,ACTIONCAT_FLOW_CONTROL,TEXT("Conditional Jump"),TEXT("Jump %p Actions if %p"),
		TEXT("Skips additional actions if the given condition is true. May skip negative amounts to go back."),
		{ TYPE_INT, TYPE_BOOL },
		&Thread::ConditionalJump
	},
	{
		ACTION_LOOP, ACTIONCAT_FLOW_CONTROL, TEXT("Loop"), TEXT("Loop"),
		TEXT("Loops the subactions. Can be exited using break or repeated by using continue actions"),
		{ },
		&Thread::ShouldNotBeImplemented
	},
	{
		ACTION_CONDBREAK, ACTIONCAT_FLOW_CONTROL, TEXT("Break If"), TEXT("Break If %p"),
		TEXT("Breaks out of the enclosing Loop action if the given condition is true. No effect if no loop is around."),
		{ TYPE_BOOL },
		&Thread::ShouldNotBeImplemented
	},
	{
		ACTION_CONDCONTINUE, ACTIONCAT_FLOW_CONTROL, TEXT("Continue If"), TEXT("Continue If %p"),
		TEXT("Goes back to the loop header of the enclosing Loop action if the given condition is true. No effect if no loop is around."),
		{ TYPE_BOOL },
		&Thread::ShouldNotBeImplemented
	},
	{
		ACTION_FORLOOP, ACTIONCAT_FLOW_CONTROL, TEXT("For Loop"), TEXT("For %p from %p till %p (exclusive)"),
		TEXT("Sets the given integer variable to the start value, then loops the subactions, increasing the variable by one after each pass, "
		"until the integer is greater or equal to the target value. Break and Continue may be used."),
		{ TYPE_INVALID, TYPE_INT, TYPE_INT },
		&Thread::ShouldNotBeImplemented
	},
	{
		10, ACTIONCAT_MODIFY_CARD, TEXT("Switch AAU Data Set"), TEXT("Switch %p 's AAU Data Set to %p"), TEXT("TODO: add description"),
		{ TYPE_INT, TYPE_INT },
		&Thread::SwitchAAUDataSet
	},

	{
		11, ACTIONCAT_FLOW_CONTROL, TEXT("End Execution"), TEXT("End Execution"), TEXT("ends execution of this thread. think of a return statement."),
		{ },
		&Thread::EndExecution
	},
	{
		12, ACTIONCAT_MODIFY_CHARACTER, TEXT("Add Love Points"), TEXT("Add %p love points for character %p towards %p"),
		TEXT("Adds a certain amount of love points. 30 love points become one love interaction. A character can have up to 30 interactions "
		"in total; after that, earlier interactions will be replaced."),
		{ TYPE_INT, TYPE_INT, TYPE_INT },
		&Thread::AddCardLovePoints
	},
	{
		13, ACTIONCAT_MODIFY_CHARACTER, TEXT("Add Like Points"), TEXT("Add %p like points for character %p towards %p"),
		TEXT("Adds a certain amount of like points. 30 like points become one like interaction. A character can have up to 30 interactions "
		"in total; after that, earlier interactions will be replaced."),
		{ TYPE_INT, TYPE_INT, TYPE_INT },
		&Thread::AddCardLikePoints
	},
	{
		14, ACTIONCAT_MODIFY_CHARACTER, TEXT("Add Dislike Points"), TEXT("Add %p dislike points for character %p towards %p"),
		TEXT("Adds a certain amount of dislike points. 30 dislike points become one dislike interaction. A character can have up to 30 interactions "
		"in total; after that, earlier interactions will be replaced."),
		{ TYPE_INT, TYPE_INT, TYPE_INT },
		&Thread::AddCardDislikePoints
	},
	{
		15, ACTIONCAT_MODIFY_CHARACTER, TEXT("Add Hate Points"), TEXT("Add %p hate points for character %p towards %p"),
		TEXT("Adds a certain amount of hate points. 30 hate points become one hate interaction. A character can have up to 30 interactions "
		"in total; after that, earlier interactions will be replaced."),
		{ TYPE_INT, TYPE_INT, TYPE_INT },
		&Thread::AddCardHatePoints
	},
	{
		16, ACTIONCAT_MODIFY_CHARACTER, TEXT("Add Points"), TEXT("Add %p %p points for character %p towards %p"),
		TEXT("Adds a certain amount of points. Point type is between 0 or 3, or use one of the named constants. "
		"30 hate points become one hate interaction. A character can have up to 30 interactions "
		"in total; after that, earlier interactions will be replaced."),
		{ TYPE_INT, TYPE_INT, TYPE_INT, TYPE_INT },
		&Thread::AddCardPoints
	},
	{
		17, ACTIONCAT_FLOW_CONTROL, TEXT("Conditional End Execution"), TEXT("End Execution If %p"),
		TEXT("ends execution of this thread if the given condition evaluates to true."),
		{ TYPE_BOOL },
		&Thread::ConditionalEndExecution
	},
	{
		18, ACTIONCAT_EVENT, TEXT("Set Npc Response Answer"), TEXT("Set Npc Response Answer to %p"),
		TEXT("When executed with a Npc Answers Event, this can be used to modify the answer the character will do."),
		{ TYPE_BOOL },
		&Thread::SetNpcResponseAnswer
	},
	{
		19, ACTIONCAT_EVENT, TEXT("Set Npc Response Percent"), TEXT("Set Npc Response Percent to %p"),
		TEXT("When executed with a Npc Answers Event, this can be used to modify the success percentage showed. Note that changing this value "
		"does not influence the Nps Answer, as it has allready been made. This Action only modifies the Percentage displayed in the UI."),
		{ TYPE_INT },
		&Thread::SetNpcResponsePercent
	},
	{
		20, ACTIONCAT_NPCACTION, TEXT("Make Npc Move to Room"), TEXT("Make Npc %p move to room %p"),
		TEXT("If the target character is controlled by the Computer, this Action makes them walk to the specified Room. "
		"If the Character is allready walking somewhere, it will do this instead. "
		"Keep in mind that executing this Action will throw an event next tick; watch out for endless loops"),
		{ TYPE_INT, TYPE_INT },
		&Thread::NpcMoveRoom
	},
	{
		21, ACTIONCAT_NPCACTION, TEXT("Make Npc do Action with no Target"), TEXT("Make Npc %p do %p"),
		TEXT("If the target character is controlled by the Computer, this Action makes them do an Action that does not require another character to execute. "
		"If the Character is allready walking somewhere, it will do this instead. "
		"Keep in mind that executing this Action will throw an event next tick; watch out for endless loops"),
		{ TYPE_INT, TYPE_INT },
		&Thread::NpcActionNoTarget
	},
	{
		22, ACTIONCAT_NPCACTION, TEXT("Make Npc Talk to Character"), TEXT("Make Npc %p converse %p with %p"),
		TEXT("If the target character is controlled by the Computer, this Action makes them walk to and start the given conversation with the target. "
		"If the Character is allready walking somewhere, it will do this instead. "
		"Keep in mind that executing this Action will throw an event next tick; watch out for endless loops"),
		{ TYPE_INT, TYPE_INT, TYPE_INT },
		&Thread::NpcTalkTo
	},
	{
		23, ACTIONCAT_NPCACTION, TEXT("Make Npc Talk to Character about someone"), TEXT("Make Npc %p converse %p with %p about %p"),
		TEXT("If the target character is controlled by the Computer, this Action makes them walk to and start the given conversation with the target about "
		"another character in class, such as asking for their opinion or spreading bad rumors. "
		"If the Character is allready walking somewhere, it will do this instead. "
		"Keep in mind that executing this Action will throw an event next tick; watch out for endless loops"),
		{ TYPE_INT, TYPE_INT, TYPE_INT, TYPE_INT },
		&Thread::NpcTalkToAbout
	},
};




ParameterisedAction::ParameterisedAction(DWORD actionId,const std::vector<ParameterisedExpression>& params) {
	this->action = Action::FromId(actionId);
	this->actualParameters = params;
}


}
}