; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [16 x i8] c"Condition true\0A\00", align 1
@1 = private unnamed_addr constant [17 x i8] c"Condition false\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %condition = alloca i1, align 1
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i1, align 1
  %3 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i32 3, i32* %0, align 4
  store i32 3, i32* %1, align 4
  %4 = load i32, i32* %0, align 4
  %5 = load i32, i32* %1, align 4
  %6 = icmp eq i32 %4, %5
  store i1 %6, i1* %2, align 1
  %7 = load i1, i1* %2, align 1
  store i1 %7, i1* %condition, align 1
  %8 = load i1, i1* %condition, align 1
  br i1 %8, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @0, i32 0, i32 0))
  store i32 0, i32* %3, align 4
  %10 = load i32, i32* %3, align 4
  ret i32 %10

if.end:                                           ; preds = %entry
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @1, i32 0, i32 0))
  %12 = load i32, i32* %result, align 4
  ret i32 %12
}
