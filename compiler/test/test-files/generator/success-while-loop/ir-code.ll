; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [16 x i8] c"i is now at: %d\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %0 = alloca i32, align 4
  store i32 0, i32* %0, align 4
  %1 = load i32, i32* %0, align 4
  %i = alloca i32, align 4
  store i32 %1, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while, %entry
  %2 = alloca i32, align 4
  store i32 10, i32* %2, align 4
  %3 = load i32, i32* %i, align 4
  %4 = load i32, i32* %2, align 4
  %5 = icmp slt i32 %3, %4
  %6 = alloca i1, align 1
  store i1 %5, i1* %6, align 1
  %7 = load i1, i1* %6, align 1
  br i1 %7, label %while, label %while.end

while:                                            ; preds = %while.cond
  %8 = alloca i32, align 4
  store i32 1, i32* %8, align 4
  %9 = load i32, i32* %8, align 4
  %10 = load i32, i32* %i, align 4
  %11 = add i32 %10, %9
  store i32 %11, i32* %i, align 4
  %12 = load i32, i32* %i, align 4
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @0, i32 0, i32 0), i32 %12)
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %14 = load i32, i32* %result, align 4
  ret i32 %14
}
