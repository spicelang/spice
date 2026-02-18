; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Test = type { i32 }

define private void @_ZN4Test4ctorEv(ptr noundef nonnull align 4 dereferenceable(4) %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  store i32 0, ptr %2, align 4
  %3 = load ptr, ptr %this, align 8
  %i.addr = getelementptr inbounds %struct.Test, ptr %3, i64 0, i32 0
  store i32 123, ptr %i.addr, align 4
  ret void
}

define private void @_ZN4Test4ctorERK4Test(ptr noundef nonnull align 4 dereferenceable(4) %0, ptr noundef %1) {
  %this = alloca ptr, align 8
  %other = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %other, align 8
  %3 = load ptr, ptr %this, align 8
  store i32 0, ptr %3, align 4
  %4 = load ptr, ptr %this, align 8
  %i.addr = getelementptr inbounds %struct.Test, ptr %4, i64 0, i32 0
  %5 = load ptr, ptr %other, align 8
  %i.addr1 = getelementptr inbounds %struct.Test, ptr %5, i64 0, i32 0
  %6 = load i32, ptr %i.addr1, align 4
  store i32 %6, ptr %i.addr, align 4
  ret void
}

define private void @_ZN4Test4dtorEv(ptr noundef nonnull align 4 dereferenceable(4) %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %i.addr = getelementptr inbounds %struct.Test, ptr %2, i64 0, i32 0
  store i32 0, ptr %i.addr, align 4
  ret void
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  %t = alloca %struct.Test, align 8
  store i32 0, ptr %result, align 4
  store i32 123, ptr %i, align 4
  %1 = load i32, ptr %i, align 4
  store i32 %1, ptr %i, align 4
  call void @_ZN4Test4ctorEv(ptr noundef nonnull align 4 dereferenceable(4) %t)
  call void @_ZN4Test4ctorERK4Test(ptr noundef nonnull align 4 dereferenceable(4) %t, ptr %t)
  call void @_ZN4Test4dtorEv(ptr noundef nonnull align 4 dereferenceable(4) %t)
  %2 = load i32, ptr %result, align 4
  ret i32 %2
}

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
