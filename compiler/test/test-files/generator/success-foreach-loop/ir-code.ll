; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [22 x i8] c"Item for index %d, %d\00", align 1
@1 = private unnamed_addr constant [22 x i8] c"Item for index %d, %d\00", align 1
@2 = private unnamed_addr constant [9 x i8] c"Item: %d\00", align 1
@3 = private unnamed_addr constant [9 x i8] c"Item: %d\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %intArray = alloca [7 x i32], i32 7, align 4
  %0 = alloca i32, align 4
  store i32 1, i32* %0, align 4
  %1 = load i32, i32* %0, align 4
  %2 = getelementptr inbounds [7 x i32], [7 x i32]* %intArray, i32 0, i32 0
  store i32 %1, i32* %2, align 4
  %3 = alloca i32, align 4
  store i32 5, i32* %3, align 4
  %4 = load i32, i32* %3, align 4
  %5 = getelementptr inbounds [7 x i32], [7 x i32]* %intArray, i32 0, i32 1
  store i32 %4, i32* %5, align 4
  %6 = alloca i32, align 4
  store i32 4, i32* %6, align 4
  %7 = load i32, i32* %6, align 4
  %8 = getelementptr inbounds [7 x i32], [7 x i32]* %intArray, i32 0, i32 2
  store i32 %7, i32* %8, align 4
  %9 = alloca i32, align 4
  store i32 0, i32* %9, align 4
  %10 = load i32, i32* %9, align 4
  %11 = getelementptr inbounds [7 x i32], [7 x i32]* %intArray, i32 0, i32 3
  store i32 %10, i32* %11, align 4
  %12 = alloca i32, align 4
  store i32 12, i32* %12, align 4
  %13 = load i32, i32* %12, align 4
  %14 = getelementptr inbounds [7 x i32], [7 x i32]* %intArray, i32 0, i32 4
  store i32 %13, i32* %14, align 4
  %15 = alloca i32, align 4
  store i32 12345, i32* %15, align 4
  %16 = load i32, i32* %15, align 4
  %17 = getelementptr inbounds [7 x i32], [7 x i32]* %intArray, i32 0, i32 5
  store i32 %16, i32* %17, align 4
  %18 = alloca i32, align 4
  store i32 9, i32* %18, align 4
  %19 = load i32, i32* %18, align 4
  %20 = getelementptr inbounds [7 x i32], [7 x i32]* %intArray, i32 0, i32 6
  store i32 %19, i32* %20, align 4
  %index = alloca i32, align 4
  store i32 0, i32* %index, align 4
  %item = alloca i32, align 4
  %21 = getelementptr inbounds [7 x i32], [7 x i32]* %intArray, i32 0
  %22 = load [7 x i32], [7 x i32]* %21, align 4
  %23 = load i32, i32* %index, align 4
  %24 = getelementptr inbounds [7 x i32], [7 x i32]* %21, i32 0, i32 %23
  %25 = load i32, i32* %24, align 4
  store i32 %25, i32* %item, align 4
  br label %foreach.loop

foreach.loop:                                     ; preds = %foreach.inc, %entry
  %26 = getelementptr inbounds i32, i32* %index, i32 0
  %27 = load i32, i32* %26, align 4
  %28 = getelementptr inbounds i32, i32* %item, i32 0
  %29 = load i32, i32* %28, align 4
  %30 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @0, i32 0, i32 0), i32 %27, i32 %29)
  %31 = load i32, i32* %index, align 4
  %foreach_idx_cmp = icmp slt i32 %31, 6
  br i1 %foreach_idx_cmp, label %foreach.inc, label %foreach.end

foreach.inc:                                      ; preds = %foreach.loop
  %32 = load i32, i32* %index, align 4
  %foreach_idx_inc = add i32 %32, 1
  store i32 %foreach_idx_inc, i32* %index, align 4
  %33 = getelementptr inbounds [7 x i32], [7 x i32]* %21, i32 0, i32 %foreach_idx_inc
  %34 = load i32, i32* %33, align 4
  store i32 %34, i32* %item, align 4
  br label %foreach.loop

foreach.end:                                      ; preds = %foreach.loop
  %35 = alloca i32, align 4
  store i32 2, i32* %35, align 4
  %36 = load i32, i32* %35, align 4
  %idx = alloca i32, align 4
  store i32 %36, i32* %idx, align 4
  %item1 = alloca i32, align 4
  %37 = getelementptr inbounds [7 x i32], [7 x i32]* %intArray, i32 0
  %38 = load [7 x i32], [7 x i32]* %37, align 4
  %39 = load i32, i32* %idx, align 4
  %40 = getelementptr inbounds [7 x i32], [7 x i32]* %37, i32 0, i32 %39
  %41 = load i32, i32* %40, align 4
  store i32 %41, i32* %item1, align 4
  br label %foreach.loop2

foreach.loop2:                                    ; preds = %foreach.inc4, %foreach.end
  %42 = getelementptr inbounds i32, i32* %idx, i32 0
  %43 = load i32, i32* %42, align 4
  %44 = getelementptr inbounds i32, i32* %item1, i32 0
  %45 = load i32, i32* %44, align 4
  %46 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @1, i32 0, i32 0), i32 %43, i32 %45)
  %47 = getelementptr inbounds i32, i32* %idx, i32 0
  %48 = load i32, i32* %47, align 4
  %49 = add i32 %48, 1
  store i32 %49, i32* %47, align 4
  %50 = load i32, i32* %idx, align 4
  %foreach_idx_cmp3 = icmp slt i32 %50, 6
  br i1 %foreach_idx_cmp3, label %foreach.inc4, label %foreach.end6

foreach.inc4:                                     ; preds = %foreach.loop2
  %51 = load i32, i32* %idx, align 4
  %foreach_idx_inc5 = add i32 %51, 1
  store i32 %foreach_idx_inc5, i32* %idx, align 4
  %52 = getelementptr inbounds [7 x i32], [7 x i32]* %37, i32 0, i32 %foreach_idx_inc5
  %53 = load i32, i32* %52, align 4
  store i32 %53, i32* %item1, align 4
  br label %foreach.loop2

foreach.end6:                                     ; preds = %foreach.loop2
  %idx7 = alloca i32, align 4
  store i32 0, i32* %idx7, align 4
  %item8 = alloca i32, align 4
  %54 = getelementptr inbounds [7 x i32], [7 x i32]* %intArray, i32 0
  %55 = load [7 x i32], [7 x i32]* %54, align 4
  %56 = load i32, i32* %idx7, align 4
  %57 = getelementptr inbounds [7 x i32], [7 x i32]* %54, i32 0, i32 %56
  %58 = load i32, i32* %57, align 4
  store i32 %58, i32* %item8, align 4
  br label %foreach.loop9

foreach.loop9:                                    ; preds = %foreach.inc11, %foreach.end6
  %59 = getelementptr inbounds i32, i32* %idx7, i32 0
  %60 = load i32, i32* %59, align 4
  %61 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @2, i32 0, i32 0), i32 %60)
  %62 = getelementptr inbounds i32, i32* %item8, i32 0
  %63 = load i32, i32* %62, align 4
  %64 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @3, i32 0, i32 0), i32 %63)
  %65 = load i32, i32* %idx7, align 4
  %foreach_idx_cmp10 = icmp slt i32 %65, 6
  br i1 %foreach_idx_cmp10, label %foreach.inc11, label %foreach.end13

foreach.inc11:                                    ; preds = %foreach.loop9
  %66 = load i32, i32* %idx7, align 4
  %foreach_idx_inc12 = add i32 %66, 1
  store i32 %foreach_idx_inc12, i32* %idx7, align 4
  %67 = getelementptr inbounds [7 x i32], [7 x i32]* %54, i32 0, i32 %foreach_idx_inc12
  %68 = load i32, i32* %67, align 4
  store i32 %68, i32* %item8, align 4
  br label %foreach.loop9

foreach.end13:                                    ; preds = %foreach.loop9
  %69 = load i32, i32* %result, align 4
  ret i32 %69
}
