class Ecef:
    x: float
    y: float
    z: float

    def __init__(self, x: int, y: int, z: int): ...
    
    @staticmethod
    def distance(pos1: Ecef, pos2: Ecef) -> float: ...


class Lla:
    longitude: float
    latitude: float
    altitude: float

    def __init__(self, longitude: float, latitude: float, altitude: float): ...

    def to_ecef(self) -> Ecef: ...


class ConstellationConfig:
    num_orbits: int
    num_satellites_per_orbit: int
    inclination: float
    mean_motion: float
    max_connection_count: int
    max_connection_distance: float

    def __init__(self, num_orbits: int, num_satellites_per_orbit: int, inclination: float, mean_motion: float, max_connection_count: int, max_connection_distance: float): ...


class GatewayScorerConfig:
    cst_config: ConstellationConfig
    time_slices: list[float]
    comm_pairs: list[tuple[Lla, Lla]]
    max_path_count: int
    alpha: float

    def __init__(self, cst_config: ConstellationConfig, time_slices: list[float], comm_pairs: list[tuple[Lla, Lla]], max_path_count: int, alpha: float): ...


class GatewayScorer:
    def __init__(self, config: GatewayScorerConfig): ...

    def calc_score(self, gateways: list[Lla]) -> float: ...
