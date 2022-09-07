; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__Vector__bool_string = type { i1, ptr }

@0 = private unnamed_addr constant [19 x i8] c"Destructor called!\00", align 1
@1 = private unnamed_addr constant [5 x i8] c"Test\00", align 1
@2 = private unnamed_addr constant [16 x i8] c"Fields: %d, %s\0A\00", align 1

define internal void @_mp__Vector__dtor(ptr %0) {
entry.l6:
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %1 = call i32 (ptr, ...) @printf(ptr @0)
  ret void
}

declare i32 @printf(ptr, ...)

define i32 @main() {
entry.l10:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %0 = alloca %_s__Vector__bool_string, align 8
  %1 = getelementptr inbounds %_s__Vector__bool_string, ptr %0, i32 0, i32 0
  store i1 true, ptr %1, align 1
  %2 = getelementptr inbounds %_s__Vector__bool_string, ptr %0, i32 0, i32 1
  store ptr @1, ptr %2, align 8
  %3 = getelementptr inbounds %_s__Vector__bool_string, ptr %0, i32 0, i32 0
  %4 = load i1, ptr %3, align 1
  %5 = zext i1 %4 to i32
  %6 = getelementptr inbounds %_s__Vector__bool_string, ptr %0, i32 0, i32 1
  %7 = load ptr, ptr %6, align 8
  %8 = call i32 (ptr, ...) @printf(ptr @2, i32 %5, ptr %7)
  br label %cleanup.l10

cleanup.l10:                                      ; preds = %entry.l10
  call void @_mp__Vector__dtor(ptr %0)
  %9 = load i32, ptr %result, align 4
  ret i32 %9
}
