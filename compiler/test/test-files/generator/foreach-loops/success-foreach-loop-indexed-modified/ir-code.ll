; ModuleID = 'source.spice'
source_filename = "source.spice"

@intArray = dso_local constant [7 x i32] [i32 1, i32 5, i32 4, i32 0, i32 12, i32 12345, i32 9]
@0 = private unnamed_addr constant [23 x i8] c"Item for index %d, %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %intArray = alloca [7 x i32], align 4
  %0 = alloca [7 x i32], align 4
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %idx = alloca i32, align 4
  %8 = alloca i32, align 4
  %item = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i32 1, i32* %1, align 4
  store i32 5, i32* %2, align 4
  store i32 4, i32* %3, align 4
  store i32 0, i32* %4, align 4
  store i32 12, i32* %5, align 4
  store i32 12345, i32* %6, align 4
  store i32 9, i32* %7, align 4
  %9 = load [7 x i32], [7 x i32]* @intArray, align 4
  store [7 x i32] %9, [7 x i32]* %intArray, align 4
  store i32 2, i32* %8, align 4
  %10 = load i32, i32* %8, align 4
  store i32 %10, i32* %idx, align 4
  %11 = load [7 x i32], [7 x i32]* %intArray, align 4
  %12 = load i32, i32* %idx, align 4
  %13 = getelementptr inbounds [7 x i32], [7 x i32]* %intArray, i32 0, i32 %12
  %14 = load i32, i32* %13, align 4
  store i32 %14, i32* %item, align 4
  br label %foreach.loop

foreach.loop:                                     ; preds = %foreach.inc, %entry
  %15 = load i32, i32* %idx, align 4
  %16 = load i32, i32* %item, align 4
  %17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @0, i32 0, i32 0), i32 %15, i32 %16)
  %18 = load i32, i32* %idx, align 4
  %19 = add i32 %18, 1
  store i32 %19, i32* %idx, align 4
  %20 = load i32, i32* %idx, align 4
  %foreach_idx_cmp = icmp slt i32 %20, 6
  br i1 %foreach_idx_cmp, label %foreach.inc, label %foreach.end

foreach.inc:                                      ; preds = %foreach.loop
  %21 = load i32, i32* %idx, align 4
  %foreach_idx_inc = add i32 %21, 1
  store i32 %foreach_idx_inc, i32* %idx, align 4
  %22 = getelementptr inbounds [7 x i32], [7 x i32]* %intArray, i32 0, i32 %foreach_idx_inc
  %23 = load i32, i32* %22, align 4
  store i32 %23, i32* %item, align 4
  br label %foreach.loop

foreach.end:                                      ; preds = %foreach.loop
  %24 = load i32, i32* %result, align 4
  ret i32 %24
}
