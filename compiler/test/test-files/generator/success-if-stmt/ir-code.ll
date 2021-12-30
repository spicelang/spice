; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [15 x i8] c"Condition true\00", align 1
@1 = private unnamed_addr constant [16 x i8] c"Condition false\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %0 = alloca i32, align 4
  store i32 3, i32* %0, align 4
  %1 = alloca i32, align 4
  store i32 3, i32* %1, align 4
  %2 = load i32, i32* %0, align 4
  %3 = load i32, i32* %1, align 4
  %4 = icmp eq i32 %2, %3
  %5 = alloca i1, align 1
  store i1 %4, i1* %5, align 1
  %6 = load i1, i1* %5, align 1
  %condition = alloca i1, align 1
  store i1 %6, i1* %condition, align 1
  %7 = getelementptr inbounds i1, i1* %condition, i32 0
  %8 = load i1, i1* %7, align 1
  br i1 %8, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @0, i32 0, i32 0))
  %10 = alloca i32, align 4
  store i32 0, i32* %10, align 4
  %11 = load i32, i32* %10, align 4
  ret i32 %11

if.end:                                           ; preds = %entry
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @1, i32 0, i32 0))
  %13 = load i32, i32* %result, align 4
  ret i32 %13
}
