; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.Inner = type { ptr }
%struct.Middle = type { %struct.Inner }
%struct.Outer = type { %struct.Middle }

@anon.string.0 = private unnamed_addr constant [12 x i8] c"Hello World\00", align 1
@printf.str.0 = private unnamed_addr constant [13 x i8] c"Message: %s\0A\00", align 1

; Function Attrs: norecurse
define private void @_ZN5Inner4ctorEv(ptr noundef nonnull align 8 dereferenceable(8) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %3 = getelementptr inbounds %struct.Inner, ptr %2, i32 0, i32 0
  store ptr @anon.string.0, ptr %3, align 8
  ret void
}

; Function Attrs: norecurse
define private void @_ZN6Middle4ctorEv(ptr noundef nonnull align 8 dereferenceable(8) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %3 = getelementptr inbounds %struct.Middle, ptr %2, i32 0, i32 0
  call void @_ZN5Inner4ctorEv(ptr %3)
  ret void
}

; Function Attrs: norecurse
define private void @_ZN5Outer4ctorEv(ptr noundef nonnull align 8 dereferenceable(8) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %3 = getelementptr inbounds %struct.Outer, ptr %2, i32 0, i32 0
  call void @_ZN6Middle4ctorEv(ptr %3)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 {
  %result = alloca i32, align 4
  %outer = alloca %struct.Outer, align 8
  store i32 0, ptr %result, align 4
  call void @_ZN5Outer4ctorEv(ptr %outer)
  %middle_addr = getelementptr inbounds %struct.Outer, ptr %outer, i32 0, i32 0
  %inner_addr = getelementptr inbounds %struct.Middle, ptr %middle_addr, i32 0, i32 0
  %message_addr = getelementptr inbounds %struct.Inner, ptr %inner_addr, i32 0, i32 0
  %1 = load ptr, ptr %message_addr, align 8
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr %1)
  %3 = load i32, ptr %result, align 4
  ret i32 %3
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #2

attributes #0 = { norecurse }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { nofree nounwind }
