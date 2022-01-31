; ModuleID = 'source.spice'
source_filename = "source.spice"

@intArray = dso_local constant [7 x i32] [i32 1, i32 5, i32 4, i32 0, i32 12, i32 12345, i32 9]
@0 = private unnamed_addr constant [22 x i8] c"Item for index %d, %d\00", align 1
@1 = private unnamed_addr constant [22 x i8] c"Item for index %d, %d\00", align 1
@2 = private unnamed_addr constant [9 x i8] c"Item: %d\00", align 1
@3 = private unnamed_addr constant [9 x i8] c"Item: %d\00", align 1

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
  %index = alloca i32, align 4
  %item = alloca i32, align 4
  %idx = alloca i32, align 4
  %8 = alloca i32, align 4
  %item1 = alloca i32, align 4
  %idx7 = alloca i32, align 4
  %item8 = alloca i32, align 4
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
  store i32 0, i32* %index, align 4
  %10 = load [7 x i32], [7 x i32]* %intArray, align 4
  %11 = load i32, i32* %index, align 4
  %12 = getelementptr inbounds [7 x i32], [7 x i32]* %intArray, i32 0, i32 %11
  %13 = load i32, i32* %12, align 4
  store i32 %13, i32* %item, align 4
  br label %foreach.loop

foreach.loop:                                     ; preds = %foreach.inc, %entry
  %14 = load i32, i32* %index, align 4
  %15 = load i32, i32* %item, align 4
  %16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @0, i32 0, i32 0), i32 %14, i32 %15)
  %17 = load i32, i32* %index, align 4
  %foreach_idx_cmp = icmp slt i32 %17, 6
  br i1 %foreach_idx_cmp, label %foreach.inc, label %foreach.end

foreach.inc:                                      ; preds = %foreach.loop
  %18 = load i32, i32* %index, align 4
  %foreach_idx_inc = add i32 %18, 1
  store i32 %foreach_idx_inc, i32* %index, align 4
  %19 = getelementptr inbounds [7 x i32], [7 x i32]* %intArray, i32 0, i32 %foreach_idx_inc
  %20 = load i32, i32* %19, align 4
  store i32 %20, i32* %item, align 4
  br label %foreach.loop

foreach.end:                                      ; preds = %foreach.loop
  store i32 2, i32* %8, align 4
  %21 = load i32, i32* %8, align 4
  store i32 %21, i32* %idx, align 4
  %22 = load [7 x i32], [7 x i32]* %intArray, align 4
  %23 = load i32, i32* %idx, align 4
  %24 = getelementptr inbounds [7 x i32], [7 x i32]* %intArray, i32 0, i32 %23
  %25 = load i32, i32* %24, align 4
  store i32 %25, i32* %item1, align 4
  br label %foreach.loop2

foreach.loop2:                                    ; preds = %foreach.inc4, %foreach.end
  %26 = load i32, i32* %idx, align 4
  %27 = load i32, i32* %item1, align 4
  %28 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @1, i32 0, i32 0), i32 %26, i32 %27)
  %29 = load i32, i32* %idx, align 4
  %30 = add i32 %29, 1
  store i32 %30, i32* %idx, align 4
  %31 = load i32, i32* %idx, align 4
  %foreach_idx_cmp3 = icmp slt i32 %31, 6
  br i1 %foreach_idx_cmp3, label %foreach.inc4, label %foreach.end6

foreach.inc4:                                     ; preds = %foreach.loop2
  %32 = load i32, i32* %idx, align 4
  %foreach_idx_inc5 = add i32 %32, 1
  store i32 %foreach_idx_inc5, i32* %idx, align 4
  %33 = getelementptr inbounds [7 x i32], [7 x i32]* %intArray, i32 0, i32 %foreach_idx_inc5
  %34 = load i32, i32* %33, align 4
  store i32 %34, i32* %item1, align 4
  br label %foreach.loop2

foreach.end6:                                     ; preds = %foreach.loop2
  store i32 0, i32* %idx7, align 4
  %35 = load [7 x i32], [7 x i32]* %intArray, align 4
  %36 = load i32, i32* %idx7, align 4
  %37 = getelementptr inbounds [7 x i32], [7 x i32]* %intArray, i32 0, i32 %36
  %38 = load i32, i32* %37, align 4
  store i32 %38, i32* %item8, align 4
  br label %foreach.loop9

foreach.loop9:                                    ; preds = %foreach.inc11, %foreach.end6
  %39 = load i32, i32* %idx7, align 4
  %40 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @2, i32 0, i32 0), i32 %39)
  %41 = load i32, i32* %item8, align 4
  %42 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @3, i32 0, i32 0), i32 %41)
  %43 = load i32, i32* %idx7, align 4
  %foreach_idx_cmp10 = icmp slt i32 %43, 6
  br i1 %foreach_idx_cmp10, label %foreach.inc11, label %foreach.end13

foreach.inc11:                                    ; preds = %foreach.loop9
  %44 = load i32, i32* %idx7, align 4
  %foreach_idx_inc12 = add i32 %44, 1
  store i32 %foreach_idx_inc12, i32* %idx7, align 4
  %45 = getelementptr inbounds [7 x i32], [7 x i32]* %intArray, i32 0, i32 %foreach_idx_inc12
  %46 = load i32, i32* %45, align 4
  store i32 %46, i32* %item8, align 4
  br label %foreach.loop9

foreach.end13:                                    ; preds = %foreach.loop9
  %47 = load i32, i32* %result, align 4
  ret i32 %47
}
