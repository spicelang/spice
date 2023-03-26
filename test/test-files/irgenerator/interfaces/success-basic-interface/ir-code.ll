; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__Car__bool = type { i1 }

@printf.str.0 = private unnamed_addr constant [15 x i8] c"Is driving: %d\00", align 1

define dso_local void @_mp__Car__void__ctor(ptr noundef nonnull %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %driving = getelementptr inbounds %__Car__bool, ptr %2, i32 0, i32 0
  store i1 false, ptr %driving, align 1
  ret void
}

define dso_local void @_mp__Car__void__drive__int(ptr noundef nonnull %0, i32 %1) {
  %this = alloca ptr, align 8
  %param = alloca i32, align 4
  store ptr %0, ptr %this, align 8
  store i32 %1, ptr %param, align 4
  %3 = load ptr, ptr %this, align 8
  %driving = getelementptr inbounds %__Car__bool, ptr %3, i32 0, i32 0
  store i1 true, ptr %driving, align 1
  ret void
}

define dso_local i1 @_mf__Car__bool__isDriving(ptr noundef nonnull %0) {
  %result = alloca i1, align 1
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %driving = getelementptr inbounds %__Car__bool, ptr %2, i32 0, i32 0
  %3 = load i1, ptr %driving, align 1
  ret i1 %3
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %1 = alloca %__Car__bool, align 8
  %car = alloca %__Car__bool, align 8
  store i32 0, ptr %result, align 4
  call void @_mp__Car__void__ctor(ptr %1)
  %2 = load %__Car__bool, ptr %1, align 1
  store %__Car__bool %2, ptr %car, align 1
  call void @_mp__Car__void__drive__int(ptr %car, i32 12)
  %3 = call i1 @_mf__Car__bool__isDriving(ptr %car)
  %4 = zext i1 %3 to i32
  %5 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %4)
  %6 = load i32, ptr %result, align 4
  ret i32 %6
}

declare i32 @printf(ptr noundef, ...)

attributes #0 = { noinline nounwind optnone uwtable }
