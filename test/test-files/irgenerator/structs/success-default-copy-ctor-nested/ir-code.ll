; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.Inner = type { i16 }
%struct.Middle = type { %struct.Inner }
%struct.Outer = type { %struct.Middle }

@printf.str.0 = private unnamed_addr constant [8 x i8] c"x = %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [8 x i8] c"x = %d\0A\00", align 1

define private void @_ZN5Inner4ctorERK5Inner(ptr noundef nonnull align 2 dereferenceable(2) %0, ptr %1) {
  %this = alloca ptr, align 8
  %other = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %other, align 8
  %3 = load ptr, ptr %this, align 8
  %4 = getelementptr inbounds %struct.Inner, ptr %3, i32 0, i32 0
  store i16 -43, ptr %4, align 2
  %5 = load ptr, ptr %this, align 8
  %x_addr = getelementptr inbounds %struct.Inner, ptr %5, i32 0, i32 0
  %6 = load ptr, ptr %other, align 8
  %x_addr1 = getelementptr inbounds %struct.Inner, ptr %6, i32 0, i32 0
  %7 = load i16, ptr %x_addr1, align 2
  %8 = add nsw i16 %7, 5
  store i16 %8, ptr %x_addr, align 2
  ret void
}

; Function Attrs: norecurse
define private void @_ZN6Middle4ctorERK6Middle(ptr noundef nonnull align 2 dereferenceable(2) %0, ptr %1) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %3 = load ptr, ptr %this, align 8
  %4 = getelementptr inbounds %struct.Middle, ptr %3, i32 0, i32 0
  %5 = load ptr, ptr %this, align 8
  %6 = getelementptr inbounds %struct.Middle, ptr %5, i32 0, i32 0
  call void @_ZN5Inner4ctorERK5Inner(ptr %6, ptr %4)
  ret void
}

; Function Attrs: norecurse
define private void @_ZN5Outer4ctorERK5Outer(ptr noundef nonnull align 2 dereferenceable(2) %0, ptr %1) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %3 = load ptr, ptr %this, align 8
  %4 = getelementptr inbounds %struct.Outer, ptr %3, i32 0, i32 0
  %5 = load ptr, ptr %this, align 8
  %6 = getelementptr inbounds %struct.Outer, ptr %5, i32 0, i32 0
  call void @_ZN6Middle4ctorERK6Middle(ptr %6, ptr %4)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 {
  %result = alloca i32, align 4
  %outer = alloca %struct.Outer, align 8
  %outer2 = alloca %struct.Outer, align 8
  store i32 0, ptr %result, align 4
  store %struct.Outer { %struct.Middle { %struct.Inner { i16 -43 } } }, ptr %outer, align 2
  %middle_addr = getelementptr inbounds %struct.Outer, ptr %outer, i32 0, i32 0
  %inner_addr = getelementptr inbounds %struct.Middle, ptr %middle_addr, i32 0, i32 0
  %x_addr = getelementptr inbounds %struct.Inner, ptr %inner_addr, i32 0, i32 0
  %1 = load i16, ptr %x_addr, align 2
  %2 = sext i16 %1 to i32
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %2)
  call void @_ZN5Outer4ctorERK5Outer(ptr %outer2, ptr %outer)
  %middle_addr1 = getelementptr inbounds %struct.Outer, ptr %outer2, i32 0, i32 0
  %inner_addr2 = getelementptr inbounds %struct.Middle, ptr %middle_addr1, i32 0, i32 0
  %x_addr3 = getelementptr inbounds %struct.Inner, ptr %inner_addr2, i32 0, i32 0
  %4 = load i16, ptr %x_addr3, align 2
  %5 = sext i16 %4 to i32
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %5)
  %7 = load i32, ptr %result, align 4
  ret i32 %7
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #2

attributes #0 = { norecurse }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { nofree nounwind }
