import grid

scorer = grid.GatewayScorer(grid.GatewayScorerConfig(
    cst_config=grid.ConstellationConfig(
        num_orbits=72,
        num_satellites_per_orbit=22,
        inclination=53.0,
        mean_motion=15.09,
        max_connection_count=4,
        max_connection_distance=1123000.0,
    ),
    time_slices=[0.0, 1.0, 2.0, 3.0, 4.0],
    comm_pairs=[
        (grid.Lla(longitude=0.0, latitude=0.0), grid.Lla(longitude=0.0, latitude=50.0)),
        (grid.Lla(longitude=0.0, latitude=0.0), grid.Lla(longitude=50.0, latitude=0.0)),
    ],
    max_path_count=5,
    alpha=0.5,
))

score = scorer.calc_score([
    grid.Lla(longitude=0.0, latitude=0.0),
    grid.Lla(longitude=0.0, latitude=50.0),
    grid.Lla(longitude=50.0, latitude=0.0),
])

print(score)
