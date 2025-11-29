; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Inner = type { i16 }
%struct.Outer = type { %struct.Middle }
%struct.Middle = type { %struct.Inner }

@printf.str.0 = private unnamed_addr constant [8 x i8] c"x = %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [8 x i8] c"x = %d\0A\00", align 4

define private void @_ZN5Inner4ctorERK5Inner(ptr noundef nonnull align 2 dereferenceable(2) %0, ptr noundef %1) {
  %this = alloca ptr, align 8
  %other = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %other, align 8
  %3 = load ptr, ptr %this, align 8
  store i16 -43, ptr %3, align 2
  %4 = load ptr, ptr %this, align 8
  %x.addr = getelementptr inbounds %struct.Inner, ptr %4, i64 0, i32 0
  %5 = load ptr, ptr %other, align 8
  %x.addr1 = getelementptr inbounds %struct.Inner, ptr %5, i64 0, i32 0
  %6 = load i16, ptr %x.addr1, align 2
  %7 = add nsw i16 %6, 5
  store i16 %7, ptr %x.addr, align 2
  ret void
}

; Function Attrs: norecurse
define void @_ZN6Middle4ctorERK6Middle(ptr noundef nonnull align 2 dereferenceable(2) %0, ptr %1) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %3 = load ptr, ptr %this, align 8
  call void @_ZN5Inner4ctorERK5Inner(ptr noundef nonnull align 2 dereferenceable(2) %3, ptr %1)
  ret void
}

; Function Attrs: norecurse
define void @_ZN5Outer4ctorERK5Outer(ptr noundef nonnull align 2 dereferenceable(2) %0, ptr %1) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %3 = load ptr, ptr %this, align 8
  call void @_ZN6Middle4ctorERK6Middle(ptr noundef nonnull align 2 dereferenceable(2) %3, ptr %1)
  ret void
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #1 {
  %result = alloca i32, align 4
  %outer = alloca %struct.Outer, align 8
  %outer2 = alloca %struct.Outer, align 8
  store i32 0, ptr %result, align 4
  store %struct.Outer { %struct.Middle { %struct.Inner { i16 -43 } } }, ptr %outer, align 2
  %middle.addr = getelementptr inbounds %struct.Outer, ptr %outer, i64 0, i32 0
  %inner.addr = getelementptr inbounds %struct.Middle, ptr %middle.addr, i64 0, i32 0
  %x.addr = getelementptr inbounds %struct.Inner, ptr %inner.addr, i64 0, i32 0
  %1 = load i16, ptr %x.addr, align 2
  %2 = sext i16 %1 to i32
  %3 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %2)
  call void @_ZN5Outer4ctorERK5Outer(ptr noundef nonnull align 2 dereferenceable(2) %outer2, ptr %outer)
  %middle.addr1 = getelementptr inbounds %struct.Outer, ptr %outer2, i64 0, i32 0
  %inner.addr2 = getelementptr inbounds %struct.Middle, ptr %middle.addr1, i64 0, i32 0
  %x.addr3 = getelementptr inbounds %struct.Inner, ptr %inner.addr2, i64 0, i32 0
  %4 = load i16, ptr %x.addr3, align 2
  %5 = sext i16 %4 to i32
  %6 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %5)
  %7 = load i32, ptr %result, align 4
  ret i32 %7
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

attributes #0 = { norecurse }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #2 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
