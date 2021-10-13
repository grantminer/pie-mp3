

def MovementLogic(left_sensor, right_sensor, left_speed, right_speed, threshold):
    '''
    Decides which wheels should move at what speed
    Inputs: sensor booleans, current speeds, sensor thresholds

    Outputs: wheel speeds
    '''
    cruising_speed =  100
    slowdown_speed = 70
    catchup_speed = 130
    stop_speed = 0

    'Case One: Nothing on the line'
    if left_sensor and right_sensor False:
        left_speed = cruising_speed
        right_speed = cruising_speed

    
    'Case Two: left sensor hits line'
    if left_sensor True and right_sensor False:
        right_speed = catchup_speed
        left_speed = catchup_speed

    'Case Three: right sensor hits line'
    if left_sensor False and right_sensor True:
        left_speed = catchup_speed
        right_speed = slowdown_speed

    'Case Four: both sensors hit line'
    if left_sensor True and right_sensor True:
        right_speed = stop_speed
        left_speed = stop_speed
    
    else:
        left_speed = stop_speed
        right_speed = stop_speed
        print(f"Edge Case (lb:{left_sensor},rb:{right_sensor}, ls:{left_speed}, rs:{right_speed}) ")

    'Return after looping through all cases'
    return(left_speed, right_speed)