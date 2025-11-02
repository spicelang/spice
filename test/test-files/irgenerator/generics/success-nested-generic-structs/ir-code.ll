; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Node = type { ptr, double }

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %_node = alloca %struct.Node, align 8
  store i32 0, ptr %result, align 4
  store %struct.Node zeroinitializer, ptr %_node, align 8
  %1 = load i32, ptr %result, align 4
  ret i32 %1
}

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
