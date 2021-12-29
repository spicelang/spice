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
  %2 = getelementptr inbounds i32, i32* %i, i32 0
  %3 = alloca i32, align 4
  store i32 10, i32* %3, align 4
  %4 = load i32, i32* %2, align 4
  %5 = load i32, i32* %3, align 4
  %6 = icmp slt i32 %4, %5
  %7 = alloca i1, align 1
  store i1 %6, i1* %7, align 1
  %8 = load i1, i1* %7, align 1
  br i1 %8, label %while, label %while.end

while:                                            ; preds = %while.cond
  %9 = alloca i32, align 4
  store i32 1, i32* %9, align 4
  %10 = load i32, i32* %9, align 4
  %11 = getelementptr inbounds i32, i32* %i, i32 0
  %12 = load i32, i32* %11, align 4
  %add = add i32 %12, %10
  store i32 %add, i32* %11, align 4
  %13 = getelementptr inbounds i32, i32* %i, i32 0
  %14 = load i32, i32* %13, align 4
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @0, i32 0, i32 0), i32 %14)
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %16 = load i32, i32* %result, align 4
  ret i32 %16
}
