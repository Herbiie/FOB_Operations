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
		
	ref array<EntityID> activeMarkers = new array<EntityID>();
	
	RplComponent m_pRplComponent;
	
	override void OnInit(IEntity owner) {
		super.OnInit(owner);
		if (!GetGame().InPlayMode()) return;
		m_pRplComponent = RplComponent.Cast(owner.FindComponent(RplComponent));
		if(!m_pRplComponent) Debug.Error("FOBOps_SCR_HintTrigger cannot hook to the RplComponent please add one!");
		SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		gameMode.GetOnPlayerSpawned().Insert(OnPlayerSpawned);
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
		int newX = Math.RandomInt(-150, 150)+crateLoc[0];
		int newY = crateLoc[1];
		int newZ = Math.RandomInt(-150, 150)+crateLoc[2];
		
		EntityID markerID = newMarker.GetID();
		
		activeMarkers.Insert(markerID);
		Rpc(SetLoc,markerID,newX,newY,newZ,false);
		SetLoc(markerID,newX,newY,newZ,false);
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void SetLoc(EntityID markerID, int newX, int newY, int newZ,bool isJIP)
	{
		
		Print("Here 1");
		IEntity newMarker = GetGame().GetWorld().FindEntityByID(markerID);
		if (!newMarker) return;
		
		SCR_MapDescriptorComponent mapDescriptorComponent = SCR_MapDescriptorComponent.Cast(newMarker.FindComponent(SCR_MapDescriptorComponent));
		protected MapItem m_MapItem = mapDescriptorComponent.Item();
		
		Print("Here 2");
		if (m_MapItem.IsVisible()) return;
		
		if (!isJIP) {
			SCR_HintManagerComponent hintComponent = SCR_HintManagerComponent.GetInstance();
        	hintComponent.ShowCustomHint("The cache is within 150m of the new marker!", "LOCATION HINT", 10);
		};
		
		
		Print("Here 3 -> "+newX+" "+newY+" "+newZ);
		vector newLoc = Vector(newX,newY,newZ);
		newMarker.SetOrigin(newLoc);
		
		Print("Here 4");
		m_MapItem.SetVisible(true);
		
		Print("Here 5");
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
	
	protected void OnPlayerSpawned(int playerId, IEntity controlledEntity)
	{
		GetGame().GetCallqueue().CallLater(AfterSpawn, 5000,false);
	}
	
	protected void AfterSpawn()
	{
		if(!m_pRplComponent) return;
		if(!m_pRplComponent.IsMaster()) return;
		foreach (EntityID thisMarker : activeMarkers) {
			IEntity newMarker = GetGame().GetWorld().FindEntityByID(thisMarker);
			if (!newMarker) return;
			vector markerLoc = newMarker.GetOrigin();
			int newX = markerLoc[0];
			int newY = markerLoc[1];
			int newZ = markerLoc[2];
			Rpc(SetLoc,thisMarker,newX,newY,newZ,true);
			SetLoc(thisMarker,newX,newY,newZ,true);
		}
	}
	
	external void deactiveMarkers()
	{
		if(!m_pRplComponent) return;
		if(!m_pRplComponent.IsMaster()) return;
		SCR_HintManagerComponent hintComponent = SCR_HintManagerComponent.GetInstance();
        hintComponent.ShowCustomHint("krow ot smees sihT", "DEBUG", 10);
		array<EntityID> activeMarkersTemp = activeMarkers;
		foreach (EntityID thisMarker : activeMarkersTemp) {
			activeMarkers.Remove(activeMarkers.Find(thisMarker));
			Rpc(switchOffMarker,thisMarker);
			switchOffMarker(thisMarker);
		}
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void switchOffMarker(EntityID thisMarker)
	{
		IEntity newMarker = GetGame().GetWorld().FindEntityByID(thisMarker);
		if (!newMarker) return;
		
		SCR_MapDescriptorComponent mapDescriptorComponent = SCR_MapDescriptorComponent.Cast(newMarker.FindComponent(SCR_MapDescriptorComponent));
		protected MapItem m_MapItem = mapDescriptorComponent.Item();
		
		m_MapItem.SetVisible(false);
	}
	
}