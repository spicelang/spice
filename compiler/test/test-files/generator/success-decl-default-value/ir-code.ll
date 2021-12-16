; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [17 x i8] c"Double value: %f\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %variable = alloca double, align 8
  %0 = getelementptr inbounds double, double* %variable, i32 0
  %1 = load double, double* %0, align 8
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @0, i32 0, i32 0), double %1)
  %3 = load i32, i32* %result, align 4
  ret i32 %3
}
