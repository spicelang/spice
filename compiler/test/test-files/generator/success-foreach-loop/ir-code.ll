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
  %8 = alloca i32, align 4
  %item1 = alloca i32, align 4
  %idx = alloca i32, align 4
  %item7 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i32 1, i32* %1, align 4
  store i32 5, i32* %2, align 4
  store i32 4, i32* %3, align 4
  store i32 0, i32* %4, align 4
  store i32 12, i32* %5, align 4
  store i32 12345, i32* %6, align 4
  store i32 9, i32* %7, align 4
  store i32 0, i32* %index, align 4
  %9 = load [7 x i32], [7 x i32]* @intArray, align 4
  %10 = load i32, i32* %index, align 4
  %11 = getelementptr inbounds [7 x i32], [7 x i32]* @intArray, i32 0, i32 %10
  %12 = load i32, i32* %11, align 4
  store i32 %12, i32* %item, align 4
  br label %foreach.loop

foreach.loop:                                     ; preds = %foreach.inc, %entry
  %13 = load i32, i32* %index, align 4
  %14 = load i32, i32* %item, align 4
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @0, i32 0, i32 0), i32 %13, i32 %14)
  %16 = load i32, i32* %index, align 4
  %foreach_idx_cmp = icmp slt i32 %16, 6
  br i1 %foreach_idx_cmp, label %foreach.inc, label %foreach.end

foreach.inc:                                      ; preds = %foreach.loop
  %17 = load i32, i32* %index, align 4
  %foreach_idx_inc = add i32 %17, 1
  store i32 %foreach_idx_inc, i32* %index, align 4
  %18 = getelementptr inbounds [7 x i32], [7 x i32]* @intArray, i32 0, i32 %foreach_idx_inc
  %19 = load i32, i32* %18, align 4
  store i32 %19, i32* %item, align 4
  br label %foreach.loop

foreach.end:                                      ; preds = %foreach.loop
  store i32 2, i32* %8, align 4
  %20 = load [7 x i32], [7 x i32]* @intArray, align 4
  %21 = load i32, i32* %8, align 4
  %22 = getelementptr inbounds [7 x i32], [7 x i32]* @intArray, i32 0, i32 %21
  %23 = load i32, i32* %22, align 4
  store i32 %23, i32* %item1, align 4
  br label %foreach.loop2

foreach.loop2:                                    ; preds = %foreach.inc4, %foreach.end
  %24 = load i32, i32* %8, align 4
  %25 = load i32, i32* %item1, align 4
  %26 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @1, i32 0, i32 0), i32 %24, i32 %25)
  %27 = load i32, i32* %8, align 4
  %28 = add i32 %27, 1
  store i32 %28, i32* %8, align 4
  %29 = load i32, i32* %8, align 4
  %foreach_idx_cmp3 = icmp slt i32 %29, 6
  br i1 %foreach_idx_cmp3, label %foreach.inc4, label %foreach.end6

foreach.inc4:                                     ; preds = %foreach.loop2
  %30 = load i32, i32* %8, align 4
  %foreach_idx_inc5 = add i32 %30, 1
  store i32 %foreach_idx_inc5, i32* %8, align 4
  %31 = getelementptr inbounds [7 x i32], [7 x i32]* @intArray, i32 0, i32 %foreach_idx_inc5
  %32 = load i32, i32* %31, align 4
  store i32 %32, i32* %item1, align 4
  br label %foreach.loop2

foreach.end6:                                     ; preds = %foreach.loop2
  store i32 0, i32* %idx, align 4
  %33 = load [7 x i32], [7 x i32]* @intArray, align 4
  %34 = load i32, i32* %idx, align 4
  %35 = getelementptr inbounds [7 x i32], [7 x i32]* @intArray, i32 0, i32 %34
  %36 = load i32, i32* %35, align 4
  store i32 %36, i32* %item7, align 4
  br label %foreach.loop8

foreach.loop8:                                    ; preds = %foreach.inc10, %foreach.end6
  %37 = load i32, i32* %idx, align 4
  %38 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @2, i32 0, i32 0), i32 %37)
  %39 = load i32, i32* %item7, align 4
  %40 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @3, i32 0, i32 0), i32 %39)
  %41 = load i32, i32* %idx, align 4
  %foreach_idx_cmp9 = icmp slt i32 %41, 6
  br i1 %foreach_idx_cmp9, label %foreach.inc10, label %foreach.end12

foreach.inc10:                                    ; preds = %foreach.loop8
  %42 = load i32, i32* %idx, align 4
  %foreach_idx_inc11 = add i32 %42, 1
  store i32 %foreach_idx_inc11, i32* %idx, align 4
  %43 = getelementptr inbounds [7 x i32], [7 x i32]* @intArray, i32 0, i32 %foreach_idx_inc11
  %44 = load i32, i32* %43, align 4
  store i32 %44, i32* %item7, align 4
  br label %foreach.loop8

foreach.end12:                                    ; preds = %foreach.loop8
  %45 = load i32, i32* %result, align 4
  ret i32 %45
}
