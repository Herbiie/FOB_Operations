class FOBOps_SCR_DeployAction : NO_SCR_DeploymentAction
{
	override event void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		IEntity spawnPoint = GetGame().GetWorld().FindEntityByName("CommandSpawn");
		IEntity commandVehicle = GetGame().GetWorld().FindEntityByName("CommandVehicle");
		vector commandLoc = commandVehicle.GetOrigin();
		spawnPoint.SetOrigin(commandLoc);
		super.PerformAction(pOwnerEntity,pUserEntity);
	}
}