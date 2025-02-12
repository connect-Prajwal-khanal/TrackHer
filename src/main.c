#include <stdio.h>

int predict_next_period(int last_period_day, int cycle_length) {
    return last_period_day + cycle_length; // Simple date calculation
}

int main() {
    int last_period, cycle_length;
    
    printf("Enter last period start day (e.g., 5 for 5th of the month): ");
    scanf("%d", &last_period);
    
    printf("Enter your average cycle length (e.g., 28 days): ");
    scanf("%d", &cycle_length);
    
    int next_period = predict_next_period(last_period, cycle_length);
    printf("Your predicted next period starts on day: %d\n", next_period);

    return 0;
}
    
 //   $ gcc -o predict_period src/main.c