; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Test = type { i32 }

@printf.str.0 = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN4Test4ctorEi(ptr noundef nonnull align 4 dereferenceable(4) %0, i32 noundef %1) #0 {
  %this = alloca ptr, align 8
  %value = alloca i32, align 4
  store ptr %0, ptr %this, align 8
  store i32 %1, ptr %value, align 4
  %3 = load ptr, ptr %this, align 8
  store i32 0, ptr %3, align 4
  %4 = load ptr, ptr %this, align 8
  %value.addr = getelementptr inbounds %struct.Test, ptr %4, i64 0, i32 0
  %5 = load i32, ptr %value, align 4
  store i32 %5, ptr %value.addr, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_Z7consumeO4Test(ptr noundef %0) #0 {
  %test = alloca ptr, align 8
  store ptr %0, ptr %test, align 8
  %2 = load ptr, ptr %test, align 8
  %value.addr = getelementptr inbounds %struct.Test, ptr %2, i64 0, i32 0
  %3 = load i32, ptr %value.addr, align 4
  ret i32 %3
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define noundef i32 @main() #1 {
  %t = alloca %struct.Test, align 8
  %1 = alloca %struct.Test, align 8
  %fromTemporary = alloca i32, align 4
  %fromMoved = alloca i32, align 4
  call void @_ZN4Test4ctorEi(ptr noundef nonnull align 4 dereferenceable(4) %t, i32 noundef 1)
  call void @_ZN4Test4ctorEi(ptr noundef nonnull align 4 dereferenceable(4) %1, i32 noundef 2)
  %2 = call noundef i32 @_Z7consumeO4Test(ptr noundef %1)
  store i32 %2, ptr %fromTemporary, align 4
  %3 = call noundef i32 @_Z7consumeO4Test(ptr noundef %t)
  store i32 %3, ptr %fromMoved, align 4
  %4 = load i32, ptr %fromTemporary, align 4
  %5 = load i32, ptr %fromMoved, align 4
  %6 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %4, i32 noundef %5)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #2 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
