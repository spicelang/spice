; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.TestStruct = type { %struct.Outer }
%struct.Outer = type { i32 }

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %ts = alloca %struct.TestStruct, align 8
  store i32 0, ptr %result, align 4
  call void @_ZN10TestStructI5OuterE4ctorEv(ptr noundef nonnull align 4 dereferenceable(4) %ts)
  %1 = load i32, ptr %result, align 4
  ret i32 %1
}

declare void @_ZN10TestStructI5OuterE4ctorEv(ptr)

attributes #0 = { noinline nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
