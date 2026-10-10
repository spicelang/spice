; ModuleID = 'source.spice'
source_filename = "source.spice"

$__tysan_v1_Simple_20Spice_20TBAA = comdat any

$__tysan_v1_omnipotent_20byte = comdat any

$__tysan_v1_long = comdat any

$__tysan_v1_long_o_0 = comdat any

$__tysan_v1_double_2a = comdat any

$__tysan_v1_double_2a_o_0 = comdat any

$__tysan_v1_double = comdat any

$__tysan_v1_double_o_0 = comdat any

@llvm.global_ctors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 0, ptr @tysan.module_ctor, ptr null }]
@__tysan_v1_Simple_20Spice_20TBAA = linkonce_odr constant { i64, i64, [18 x i8] } { i64 2, i64 0, [18 x i8] c"Simple Spice TBAA\00" }, comdat
@__tysan_v1_omnipotent_20byte = linkonce_odr constant { i64, i64, ptr, i64, [16 x i8] } { i64 2, i64 1, ptr @__tysan_v1_Simple_20Spice_20TBAA, i64 0, [16 x i8] c"omnipotent byte\00" }, comdat
@__tysan_v1_long = linkonce_odr constant { i64, i64, ptr, i64, [5 x i8] } { i64 2, i64 1, ptr @__tysan_v1_omnipotent_20byte, i64 0, [5 x i8] c"long\00" }, comdat
@__tysan_v1_long_o_0 = linkonce_odr constant { i64, ptr, ptr, i64 } { i64 1, ptr @__tysan_v1_long, ptr @__tysan_v1_long, i64 0 }, comdat
@__tysan_v1_double_2a = linkonce_odr constant { i64, i64, ptr, i64, [8 x i8] } { i64 2, i64 1, ptr @__tysan_v1_omnipotent_20byte, i64 0, [8 x i8] c"double*\00" }, comdat
@__tysan_v1_double_2a_o_0 = linkonce_odr constant { i64, ptr, ptr, i64 } { i64 1, ptr @__tysan_v1_double_2a, ptr @__tysan_v1_double_2a, i64 0 }, comdat
@__tysan_v1_double = linkonce_odr constant { i64, i64, ptr, i64, [7 x i8] } { i64 2, i64 1, ptr @__tysan_v1_omnipotent_20byte, i64 0, [7 x i8] c"double\00" }, comdat
@__tysan_v1_double_o_0 = linkonce_odr constant { i64, ptr, ptr, i64 } { i64 1, ptr @__tysan_v1_double, ptr @__tysan_v1_double, i64 0 }, comdat
@llvm.used = appending global [9 x ptr] [ptr @tysan.module_ctor, ptr @__tysan_v1_Simple_20Spice_20TBAA, ptr @__tysan_v1_omnipotent_20byte, ptr @__tysan_v1_long, ptr @__tysan_v1_long_o_0, ptr @__tysan_v1_double_2a, ptr @__tysan_v1_double_2a_o_0, ptr @__tysan_v1_double, ptr @__tysan_v1_double_o_0], section "llvm.metadata"
@__tysan_shadow_memory_address = external global i64
@__tysan_app_memory_mask = external global i64

; Function Attrs: mustprogress noinline norecurse nounwind optnone sanitize_type uwtable
define noundef i32 @main() #0 !dbg !9 {
  %app.mem.mask = load i64, ptr @__tysan_app_memory_mask, align 8
  %shadow.base = load i64, ptr @__tysan_shadow_memory_address, align 8
  %l = alloca i64, align 8, !type !20
  call void @__tysan_instrument_mem_inst(ptr %l, ptr null, i64 8, i1 false)
  %ptr = alloca ptr, align 8, !dbg !21, !type !22
    #dbg_declare(ptr %l, !14, !DIExpression(), !21)
  call void @__tysan_instrument_mem_inst(ptr %ptr, ptr null, i64 8, i1 false), !dbg !21
  call void @__tysan_instrument_with_shadow_update(ptr %l, ptr @__tysan_v1_long_o_0, i1 true, i64 8, i32 2), !dbg !21
  store i64 100, ptr %l, align 8, !dbg !21, !tbaa !23
    #dbg_declare(ptr %ptr, !16, !DIExpression(), !27)
  call void @__tysan_instrument_with_shadow_update(ptr %ptr, ptr @__tysan_v1_double_2a_o_0, i1 true, i64 8, i32 2), !dbg !27
  store ptr %l, ptr %ptr, align 8, !dbg !27, !tbaa !28
  call void @__tysan_instrument_with_shadow_update(ptr %ptr, ptr null, i1 true, i64 8, i32 1), !dbg !30
  %1 = load ptr, ptr %ptr, align 8, !dbg !30
  call void @__tysan_instrument_with_shadow_update(ptr %1, ptr @__tysan_v1_double_o_0, i1 true, i64 8, i32 1), !dbg !30
  %2 = load double, ptr %1, align 8, !dbg !30, !tbaa !31
  %3 = fadd double %2, 2.000000e+00, !dbg !30
  call void @__tysan_instrument_with_shadow_update(ptr %1, ptr null, i1 true, i64 8, i32 2), !dbg !30
  store double %3, ptr %1, align 8, !dbg !30
  ret i32 0, !dbg !33
}

declare void @__tysan_init()

; Function Attrs: nounwind uwtable
define internal void @tysan.module_ctor() #1 {
  call void @__tysan_init()
  ret void
}

; Function Attrs: nounwind
declare void @__tysan_check(ptr, i32, ptr, i32) #2

; Function Attrs: nounwind
declare void @__tysan_instrument_mem_inst(ptr, ptr, i64, i1) #2

; Function Attrs: nounwind
declare void @__tysan_instrument_with_shadow_update(ptr, ptr, i1, i64, i32) #2

; Function Attrs: nounwind
declare void @__tysan_set_shadow_type(ptr, ptr, i64) #2

attributes #0 = { mustprogress noinline norecurse nounwind optnone sanitize_type uwtable }
attributes #1 = { nounwind uwtable }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3, !4, !5}
!llvm.ident = !{!6}
!llvm.dbg.cu = !{!7}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{i32 7, !"Dwarf Version", i32 5}
!5 = !{i32 2, !"Debug Info Version", i32 3}
!6 = !{!"spice version dev [self-hosted] (https://github.com/spicelang/spice)"}
!7 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !8, producer: "spice version dev [self-hosted] (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!8 = !DIFile(filename: "source.spice", directory: "./test-files/irgenerator/instrumentation/success-tysan-and-dbg-info")
!9 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainv", scope: !8, file: !8, line: 3, type: !10, scopeLine: 3, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !7, retainedNodes: !13)
!10 = !DISubroutineType(types: !11)
!11 = !{!12}
!12 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!13 = !{!14, !16}
!14 = !DILocalVariable(name: "l", scope: !9, file: !8, line: 4, type: !15)
!15 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!16 = !DILocalVariable(name: "ptr", scope: !17, file: !8, line: 6, type: !18)
!17 = distinct !DILexicalBlock(scope: !9, file: !8, line: 5, column: 5)
!18 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !19, size: 64, dwarfAddressSpace: 0)
!19 = !DIBasicType(name: "double", size: 64, encoding: DW_ATE_float)
!20 = !{i64 1048441731396222360, !"long"}
!21 = !DILocation(line: 4, column: 14, scope: !9)
!22 = !{i64 1632420314169214644, !"double*"}
!23 = !{!24, !24, i64 0}
!24 = !{!"long", !25, i64 0}
!25 = !{!"omnipotent byte", !26, i64 0}
!26 = !{!"Simple Spice TBAA"}
!27 = !DILocation(line: 6, column: 37, scope: !17)
!28 = !{!29, !29, i64 0}
!29 = !{!"double*", !25, i64 0}
!30 = !DILocation(line: 7, column: 9, scope: !17)
!31 = !{!32, !32, i64 0}
!32 = !{!"double", !25, i64 0}
!33 = !DILocation(line: 9, column: 1, scope: !9)
