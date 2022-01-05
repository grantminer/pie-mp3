

def MovementLogic(left_bool, right_bool, left_speed, right_speed):
    '''
    Decides which wheels should move at what speed
    Inputs: sensor booleans, current speeds, sensor thresholds

    Outputs: wheel speeds
    '''
    cruising_speed =  100
    slowdown_speed = -30
    catchup_speed = 130
    stop_speed = 0

    #Case One: Nothing on the line
    if not left_bool and not right_bool:
        left_speed = cruising_speed
        right_speed = cruising_speed

    #Case Two: left sensor hits line
    if left_bool and not right_bool:
        right_speed = catchup_speed
        left_speed = slowdown_speed

    #Case Three: right sensor hits line
    if not left_bool and right_bool:
        left_speed = catchup_speed
        right_speed = slowdown_speed

    #Case Four: both sensors hit line
    if left_bool and right_bool:
        right_speed = stop_speed
        left_speed = stop_speed
    
    else:
        left_speed = stop_speed
        right_speed = stop_speed
        print(f"Edge Case (lb:{left_bool},rb:{right_bool}, ls:{left_speed}, rs:{right_speed}) ")

    #Return after looping through all cases
    return(left_speed, right_speed)