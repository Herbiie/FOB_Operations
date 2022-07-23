[EntityEditorProps(category: "GameScripted/Triggers", description: "Trigger that Spawns Environment or AI stuff.")]
class FOBOps_SCR_HintTriggerClass: SCR_BaseTriggerEntityClass
{
};

class FOBOps_SCR_HintTrigger : SCR_BaseTriggerEntity
{
	
	[Attribute("", UIWidgets.EditBox, "Name of the linked trigger", category: "FOB Ops:")]
	string m_sTriggerName;	
	
	[Attribute("", UIWidgets.EditBox, "Name of the linked crate", category: "FOB Ops:")]
	string m_sCrateName;	
	
	[RplProp()]
	int newX = 0;
	[RplProp()]
	int newY = 0;
	[RplProp()]
	int newZ = 0;
	
	ref array<EntityID> activeMarkers = new array<EntityID>();
	
	RplComponent m_pRplComponent;
	
	override void OnInit(IEntity owner) {
		super.OnInit(owner);
		if (!GetGame().InPlayMode()) return;
		m_pRplComponent = RplComponent.Cast(owner.FindComponent(RplComponent));
		if(!m_pRplComponent) Debug.Error("FOBOps_SCR_HintTrigger cannot hook to the RplComponent please add one!");
		SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		gameMode.GetOnPlayerRegistered().Insert(OnPlayerRegistered);
	}
	
	override bool ScriptedEntityFilterForQuery(IEntity ent) {
        SCR_ChimeraCharacter cc = SCR_ChimeraCharacter.Cast(ent);
        if (!cc) return false; // If the entity is not a person, filter it out
        if (cc.GetFactionKey() != "FIA") return false; // If the entity does not have the Faction key of FIA, filter it out
        if (IsAlive(cc)) return false; // If the entity is alive, filter it out
        return true; // Otherwise, include it!
    }
	
	override void OnActivate(IEntity ent)
	{
		IEntity triggerEntity = GetGame().GetWorld().FindEntityByName(m_sTriggerName);
		FOBOps_SCR_SpawnTrigger triggerScript = FOBOps_SCR_SpawnTrigger.Cast(triggerEntity);
		if (!triggerScript.IsActive()) return;
		
		//SCR_HintManagerComponent hintComponent = SCR_HintManagerComponent.GetInstance();
        //hintComponent.ShowCustomHint("Death! Death! DEATH!", "Debug", 10);
		
		int randomChance = Math.RandomInt(0, 100);
		if (randomChance > 10) return;
		GetLoc();
	}
	
	protected void GetLoc()
	{
		if(!m_pRplComponent) return;
		if(!m_pRplComponent.IsMaster()) return;
		
		ref array<IEntity> children = new array<IEntity>();
		GetAllChildren(this,children);
		if (!children) return;
		if ((children.Count())==0) return;
		ref array<IEntity> TempChildren = new array<IEntity>();
		foreach (IEntity child : children)
		{
			if (!child) continue;
			SCR_MapDescriptorComponent mapDescriptorComponent = SCR_MapDescriptorComponent.Cast(child.FindComponent(SCR_MapDescriptorComponent));
			protected MapItem m_MapItemTemp = mapDescriptorComponent.Item();
			
			if (mapDescriptorComponent && m_MapItemTemp && !m_MapItemTemp.IsVisible())
			{
				TempChildren.Insert(child);
			};
		}
		
		if ((TempChildren.Count())==0) return;
		IEntity newMarker = TempChildren[0];
		if (!newMarker) return;
		
		//SCR_HintManagerComponent hintComponent = SCR_HintManagerComponent.GetInstance();
        //hintComponent.ShowCustomHint("krow ot smees sihT", "DEBUG", 10);
		IEntity crateEntity = GetGame().GetWorld().FindEntityByName(m_sCrateName);
		vector crateLoc = crateEntity.GetOrigin();
		newX = Math.RandomInt(-150, 150)+crateLoc[0];
		newY = crateLoc[1];
		newZ = Math.RandomInt(-150, 150)+crateLoc[2];
		
		EntityID markerID = newMarker.GetID();
		
		Replication.BumpMe();
		activeMarkers.Insert(markerID);
		Rpc(SetLoc,markerID);
		SetLoc(markerID);
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void SetLoc(EntityID markerID)
	{
		
		IEntity newMarker = GetGame().GetWorld().FindEntityByID(markerID);
		if (!newMarker) return;
		
		SCR_MapDescriptorComponent mapDescriptorComponent = SCR_MapDescriptorComponent.Cast(newMarker.FindComponent(SCR_MapDescriptorComponent));
		protected MapItem m_MapItem = mapDescriptorComponent.Item();
		
		if (m_MapItem.IsVisible()) return;
		
		SCR_HintManagerComponent hintComponent = SCR_HintManagerComponent.GetInstance();
        hintComponent.ShowCustomHint("The cache is within 150m of the new marker!", "LOCATION HINT", 10);
		
		
		vector newLoc = Vector(newX,newY,newZ);
		newMarker.SetOrigin(newLoc);
		
		m_MapItem.SetVisible(true);
	}
	
	//Thanks to narcoleptic marshmallow for his message on the arma discord: https://discord.com/channels/105462288051380224/976155351999201390/978395568453865622 
	static void GetAllChildren(IEntity parent, notnull inout array<IEntity> allChildren)
    {
        if (!parent)
            return;
        
        IEntity child = parent.GetChildren();
        
        while (child)
        {
            allChildren.Insert(child);
            child = child.GetSibling();
        }
    }
	
	protected void OnPlayerRegistered(int playerID)
	{
		if(!m_pRplComponent) return;
		if(!m_pRplComponent.IsMaster()) return;
		foreach (EntityID thisMarker : activeMarkers) {
			Rpc(SetLoc,thisMarker);
		}
	}
	
}