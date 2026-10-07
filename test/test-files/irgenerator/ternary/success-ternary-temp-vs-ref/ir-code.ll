; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Test = type {}

@printf.str.0 = private unnamed_addr constant [7 x i8] c"Ctor!\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [12 x i8] c"Copy ctor!\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [7 x i8] c"Dtor!\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN4Test4ctorEv(ptr noundef nonnull align 1 %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN4Test4ctorERK4Test(ptr noundef nonnull align 1 %0, ptr noundef %1) #0 {
  %this = alloca ptr, align 8
  %_ = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %_, align 8
  %3 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN4Test4dtorEv(ptr noundef nonnull align 1 %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z6choosebRK4Test(ptr dead_on_unwind noalias writable sret(%struct.Test) align 1 %0, i1 noundef zeroext %1, ptr noundef %2) #0 {
  %cond = alloca i1, align 1
  %ref = alloca ptr, align 8
  %4 = alloca %struct.Test, align 8
  %5 = alloca %struct.Test, align 8
  store i1 %1, ptr %cond, align 1
  store ptr %2, ptr %ref, align 8
  %6 = load i1, ptr %cond, align 1
  br i1 %6, label %cond.true.L11C12, label %cond.false.L11C12

cond.true.L11C12:                                 ; preds = %3
  call void @_ZN4Test4ctorEv(ptr noundef nonnull align 1 %4)
  br label %cond.exit.L11C12

cond.false.L11C12:                                ; preds = %3
  %7 = load ptr, ptr %ref, align 8
  call void @_ZN4Test4ctorERK4Test(ptr noundef nonnull align 1 %5, ptr %7)
  br label %cond.exit.L11C12

cond.exit.L11C12:                                 ; preds = %cond.false.L11C12, %cond.true.L11C12
  %cond.result = phi ptr [ %4, %cond.true.L11C12 ], [ %5, %cond.false.L11C12 ]
  %8 = load %struct.Test, ptr %cond.result, align 1
  store %struct.Test %8, ptr %0, align 1
  ret void
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #2 {
  %t = alloca %struct.Test, align 8
  %viaTemp = alloca %struct.Test, align 8
  %viaRef = alloca %struct.Test, align 8
  call void @_ZN4Test4ctorEv(ptr noundef nonnull align 1 %t)
  call void @_Z6choosebRK4Test(ptr dead_on_unwind writable sret(%struct.Test) align 1 %viaTemp, i1 noundef zeroext true, ptr noundef %t)
  call void @_ZN4Test4dtorEv(ptr noundef nonnull align 1 %viaTemp)
  call void @_Z6choosebRK4Test(ptr dead_on_unwind writable sret(%struct.Test) align 1 %viaRef, i1 noundef zeroext false, ptr noundef %t)
  call void @_ZN4Test4dtorEv(ptr noundef nonnull align 1 %viaRef)
  call void @_ZN4Test4dtorEv(ptr noundef nonnull align 1 %t)
  ret i32 0
}

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { mustprogress noinline norecurse nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev [host] (https://github.com/spicelang/spice)"}
