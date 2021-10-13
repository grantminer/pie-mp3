

def seesBlack(left_analog, right_analog, threshold):
    
    if left_analog > threshold:
        left_bool = False
    else:
        left_bool = True

    if right_analog > threshold:
        right_bool = False
    else:
        right_bool = True
    
    return left_bool, right_bool
