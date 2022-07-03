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
	
	private RplComponent m_pRplComponent2;
	
	override void OnActivate(IEntity ent)
	{
		IEntity taskEntity = GetGame().GetWorld().FindEntityByName(m_sTaskName);
		NO_SCR_EditorTask taskObject = NO_SCR_EditorTask.Cast(taskEntity);
		if (!taskObject) return;
		super.OnActivate(ent);
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
	
	
	/*void MoveCrate()
	{	
		if ((m_vCrateLocations.Count())==0) return;
		vector location = m_vCrateLocations.Get(m_vCrateLocations.GetRandomIndex());
		if (!location) return;
		IEntity crateEntity = GetGame().GetWorld().FindEntityByName(m_sCrateName);
		if (!crateEntity) return;
		crateEntity.SetOrigin(location);
		Print(location);
	}*/
	
 
}
