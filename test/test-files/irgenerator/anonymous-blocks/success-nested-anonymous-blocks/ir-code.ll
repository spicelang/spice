; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [11 x i8] c"Param: %d\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z4testi(i32 noundef %0) #0 {
  %param = alloca i32, align 4
  store i32 %0, ptr %param, align 4
  %2 = load i32, ptr %param, align 4
  %3 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %2)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #2 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  call void @_Z4testi(i32 noundef 12)
  %1 = load i32, ptr %result, align 4
  ret i32 %1
}

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { mustprogress noinline norecurse nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
