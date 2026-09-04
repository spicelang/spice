; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [6 x i8] c"done\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z4noopIPhEvRPh(ptr noundef %0) #0 {
  %obj = alloca ptr, align 8
  store ptr %0, ptr %obj, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z4noopIPVhEvRPVh(ptr noundef %0) #0 {
  %obj = alloca ptr, align 8
  store ptr %0, ptr %obj, align 8
  ret void
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #1 {
  %a = alloca ptr, align 8
  %b = alloca ptr, align 8
  store ptr null, ptr %a, align 8
  store ptr null, ptr %b, align 8
  call void @_Z4noopIPhEvRPh(ptr noundef %a)
  call void @_Z4noopIPVhEvRPVh(ptr noundef %b)
  %1 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  call void @_Z8sDeallocRPVh(ptr %b)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

declare void @_Z8sDeallocRPVh(ptr)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #2 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
