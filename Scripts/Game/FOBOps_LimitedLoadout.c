//---------WORK IN PROGRESS--------------------------------------------------------------------
[BaseContainerProps(configRoot: true), BaseContainerCustomTitleField("m_sLoadoutName")]
class FOBOps_LimitedLoadout : SCR_FactionPlayerLoadout
{
	[Attribute(defvalue: "1", UIWidgets.Slider, desc: "Maximum number of loadouts allowed.", params: "1 64 1")]
	protected int m_inumberLoadout;
	
	override bool IsLoadoutAvailable(int playerId)
	{
		return CountLoadouts();
	}
	
	//------------------------------------------------------------------------------------------------
	override bool IsLoadoutAvailableClient()
	{
		return CountLoadouts();
	}
	
	protected bool CountLoadouts()
	{
		// Count the loadouts
		int loadoutCount = 0;
		array<int> PlayersArray = new array<int>();
		GetGame().GetPlayerManager().GetPlayers(PlayersArray);
		
		foreach(int PlayerId : PlayersArray )
		{
			if (!(PlayerId>0)) continue;
			IEntity PlayerEntity = GetGame().GetPlayerManager().GetPlayerControlledEntity(PlayerId);
			if (!PlayerEntity) continue;
			BaseContainer PlayerPrefab = PlayerEntity.GetPrefabData().GetPrefab();
			if (!PlayerPrefab) continue;
			ResourceName PlayerResource = SCR_BaseContainerTools.GetPrefabResourceName(PlayerPrefab);
			if (PlayerResource==m_sLoadoutResource) ++loadoutCount;
		}
		
		return (m_inumberLoadout>loadoutCount);
	}
	
}