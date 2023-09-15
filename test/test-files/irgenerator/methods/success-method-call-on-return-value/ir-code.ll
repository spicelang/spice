; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.Stamp = type { double, i1 }
%struct.Letter = type { ptr, %struct.Stamp }

@printf.str.0 = private unnamed_addr constant [21 x i8] c"Value: %f, glued: %d\00", align 1
@anon.string.0 = private unnamed_addr constant [17 x i8] c"This is a letter\00", align 1
@printf.str.1 = private unnamed_addr constant [17 x i8] c"Stamp glued: %d\0A\00", align 1

define private void @_ZN5Stamp5printEv(ptr noundef nonnull %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %value_addr = getelementptr inbounds %struct.Stamp, ptr %2, i32 0, i32 0
  %3 = load double, ptr %value_addr, align 8
  %4 = load ptr, ptr %this, align 8
  %glued_addr = getelementptr inbounds %struct.Stamp, ptr %4, i32 0, i32 1
  %5 = load i1, ptr %glued_addr, align 1
  %6 = zext i1 %5 to i32
  %7 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, double %3, i32 %6)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #0

define private %struct.Stamp @_ZN6Letter8getStampEv(ptr noundef nonnull %0) {
  %result = alloca %struct.Stamp, align 8
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %stamp_addr = getelementptr inbounds %struct.Letter, ptr %2, i32 0, i32 1
  %3 = load %struct.Stamp, ptr %stamp_addr, align 8
  ret %struct.Stamp %3
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 {
  %result = alloca i32, align 4
  %letter = alloca %struct.Letter, align 8
  %stamp = alloca %struct.Stamp, align 8
  store i32 0, ptr %result, align 4
  store %struct.Letter { ptr @anon.string.0, %struct.Stamp { double 3.400000e+00, i1 true } }, ptr %letter, align 8
  %stamp_addr = getelementptr inbounds %struct.Letter, ptr %letter, i32 0, i32 1
  %glued_addr = getelementptr inbounds %struct.Stamp, ptr %stamp_addr, i32 0, i32 1
  %1 = load i1, ptr %glued_addr, align 1
  %2 = zext i1 %1 to i32
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %2)
  %4 = call %struct.Stamp @_ZN6Letter8getStampEv(ptr %letter)
  store %struct.Stamp %4, ptr %stamp, align 8
  call void @_ZN5Stamp5printEv(ptr %stamp)
  %5 = load i32, ptr %result, align 4
  ret i32 %5
}

attributes #0 = { nofree nounwind }
attributes #1 = { noinline nounwind optnone uwtable }
