; ModuleID = 'source.spice'
source_filename = "source.spice"

@COUNTER = internal global i32 1

; Function Attrs: noinline nounwind optnone uwtable
define void @_Z9incrementv() #0 {
  %1 = load i32, ptr @COUNTER, align 4
  %2 = add nsw i32 %1, 1
  store i32 %2, ptr @COUNTER, align 4
  ret void
}

attributes #0 = { noinline nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"Code Model", i32 2}
!1 = !{i32 7, !"uwtable", i32 2}
!2 = !{i32 7, !"frame-pointer", i32 0}
!3 = !{!"spice version dev (https://github.com/spicelang/spice)"}
