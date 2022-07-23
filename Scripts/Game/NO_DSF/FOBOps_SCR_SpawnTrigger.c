[EntityEditorProps(category: "GameScripted/Triggers", description: "Trigger that Spawns Environment or AI stuff.")]
class FOBOps_SCR_SpawnTriggerClass: NO_SCR_SpawnTriggerClass
{
};

class FOBOps_SCR_SpawnTrigger : NO_SCR_SpawnTrigger
{
	
	
	[Attribute("", UIWidgets.EditBox, "Add crate spawners.", category: "FOB Ops:" )]
	ref array<string> m_sCrateSpawners;
	
	
	[Attribute("", UIWidgets.EditBox, "Name of the linked task", category: "FOB Ops:")]
	string m_sTaskName;	
	
	
	[Attribute("", UIWidgets.EditBox, "Name of the linked hint trigger", category: "FOB Ops:")]
	string m_sHintTrigger;	
	
	private RplComponent m_pRplComponent2;
	bool isTriggerActive = false;
	
	override void OnActivate(IEntity ent)
	{
		isTriggerActive = true;
		IEntity taskEntity = GetGame().GetWorld().FindEntityByName(m_sTaskName);
		NO_SCR_EditorTask taskObject = NO_SCR_EditorTask.Cast(taskEntity);
		if (!taskObject) return;
		super.OnActivate(ent);
	}
	
	override void OnDeactivate(IEntity ent)
	{
		isTriggerActive = false;
		IEntity hintTriggerEntity = GetGame().GetWorld().FindEntityByName(m_sHintTrigger);
		FOBOps_SCR_HintTrigger hintTrigger = FOBOps_SCR_HintTrigger.Cast(hintTriggerEntity);
		hintTrigger.deactiveMarkers();
		super.OnDeactivate(ent);
	}

	override void Spawn()
	{
				
		foreach (string crateSpawner : m_sCrateSpawners)
		{
			
			IEntity spawnerEntity = GetGame().GetWorld().FindEntityByName(crateSpawner);
			auto spawnerComponent = NO_SCR_EnvSpawnerComponent.Cast(spawnerEntity.FindComponent(NO_SCR_EnvSpawnerComponent));
			if ((childrenEnvSpawner.Find(spawnerComponent)) > -1) continue;
			childrenEnvSpawner.Insert(spawnerComponent);
		}
		
		string location = m_sCrateSpawners.Get(m_sCrateSpawners.GetRandomIndex());
		
		foreach (string crateSpawner : m_sCrateSpawners)
		{
			if (location == crateSpawner) continue;
			IEntity spawnerEntity = GetGame().GetWorld().FindEntityByName(crateSpawner);
			auto spawnerComponent = NO_SCR_EnvSpawnerComponent.Cast(spawnerEntity.FindComponent(NO_SCR_EnvSpawnerComponent));
			childrenEnvSpawner.Remove(childrenEnvSpawner.Find(spawnerComponent));
		}
		super.Spawn();

	}
	
	external bool IsActive()
	{
		return isTriggerActive;
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
	
	
	/*void MoveCrate()
	{	
		if ((m_vCrateLocations.Count())==0) return;
		vector location = m_vCrateLocations.Get(m_vCrateLocations.GetRandomIndex());
		if (!location) return;
		IEntity crateEntity = GetGame().GetWorld().FindEntityByName(m_sCrateName);
		if (!crateEntity) return;
		crateEntity.SetOrigin(location);
		Print(location);
	}
	
	protected void HintCheck()
	{
		SCR_HintManagerComponent hintComponent = SCR_HintManagerComponent.GetInstance();
        hintComponent.ShowCustomHint("Death! Death! DEATH!", "Debug", 10);
		int randomChance = Math.RandomInt(0, 100);
		if (randomChance < 10) return;
	}*/
 
}
