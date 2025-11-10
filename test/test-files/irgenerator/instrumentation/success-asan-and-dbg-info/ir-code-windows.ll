; ModuleID = 'source.spice'
source_filename = "source.spice"

@__asan_shadow_memory_dynamic_address = external global i64
@__asan_option_detect_stack_use_after_return = external global i32
@___asan_gen_stack = private unnamed_addr constant [14 x i8] c"1 32 8 4 iPtr\00", align 1
@llvm.used = appending global [1 x ptr] [ptr @asan.module_ctor], section "llvm.metadata"
@llvm.global_ctors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 1, ptr @asan.module_ctor, ptr null }]

; Function Attrs: mustprogress noinline norecurse nounwind optnone sanitize_address uwtable
define dso_local noundef i32 @main() #0 !dbg !10 {
  %1 = load i64, ptr @__asan_shadow_memory_dynamic_address, align 8
  %result = alloca i32, align 4
  %asan_local_stack_base = alloca i64, align 8
  %2 = load i32, ptr @__asan_option_detect_stack_use_after_return, align 4
  %3 = icmp ne i32 %2, 0
  br i1 %3, label %4, label %6

4:                                                ; preds = %0
  %5 = call i64 @__asan_stack_malloc_0(i64 64)
  br label %6

6:                                                ; preds = %0, %4
  %7 = phi i64 [ 0, %0 ], [ %5, %4 ]
  %8 = icmp eq i64 %7, 0
  br i1 %8, label %9, label %11

9:                                                ; preds = %6
  %MyAlloca = alloca i8, i64 64, align 32
  %10 = ptrtoint ptr %MyAlloca to i64
  br label %11

11:                                               ; preds = %6, %9
  %12 = phi i64 [ %7, %6 ], [ %10, %9 ]
  store i64 %12, ptr %asan_local_stack_base, align 8
  %13 = add i64 %12, 32
  %14 = inttoptr i64 %13 to ptr
  %15 = inttoptr i64 %12 to ptr
  store i64 1102416563, ptr %15, align 8
  %16 = add i64 %12, 8
  %17 = inttoptr i64 %16 to ptr
  store i64 ptrtoint (ptr @___asan_gen_stack to i64), ptr %17, align 8
  %18 = add i64 %12, 16
  %19 = inttoptr i64 %18 to ptr
  store i64 ptrtoint (ptr @main to i64), ptr %19, align 8
  %20 = lshr i64 %12, 3
  %21 = add i64 %20, %1
  %22 = add i64 %21, 0
  %23 = inttoptr i64 %22 to ptr
  store i64 -868083117767659023, ptr %23, align 1
    #dbg_declare(ptr %result, !16, !DIExpression(), !17)
  store i32 0, ptr %result, align 4, !dbg !17
  %24 = call ptr @_Z4sNewIiEPiv(), !dbg !18
  store ptr %24, ptr %14, align 8, !dbg !18
    #dbg_declare(ptr %asan_local_stack_base, !19, !DIExpression(DW_OP_deref, DW_OP_plus_uconst, 32), !21)
  %25 = load ptr, ptr %14, align 8, !dbg !22
  %26 = ptrtoint ptr %25 to i64, !dbg !23
  %27 = lshr i64 %26, 3, !dbg !23
  %28 = add i64 %27, %1, !dbg !23
  %29 = inttoptr i64 %28 to ptr, !dbg !23
  %30 = load i8, ptr %29, align 1, !dbg !23
  %31 = icmp ne i8 %30, 0, !dbg !23
  br i1 %31, label %32, label %38, !dbg !23, !prof !24

32:                                               ; preds = %11
  %33 = and i64 %26, 7, !dbg !23
  %34 = add i64 %33, 3, !dbg !23
  %35 = trunc i64 %34 to i8, !dbg !23
  %36 = icmp sge i8 %35, %30, !dbg !23
  br i1 %36, label %37, label %38, !dbg !23

37:                                               ; preds = %32
  call void @__asan_report_store4(i64 %26) #3, !dbg !23
  unreachable

38:                                               ; preds = %32, %11
  store i32 123, ptr %25, align 4, !dbg !23
  call void @_Z8sDeallocRPh(ptr %14), !dbg !25
  %39 = load ptr, ptr %14, align 8, !dbg !27
  %40 = ptrtoint ptr %39 to i64, !dbg !28
  %41 = lshr i64 %40, 3, !dbg !28
  %42 = add i64 %41, %1, !dbg !28
  %43 = inttoptr i64 %42 to ptr, !dbg !28
  %44 = load i8, ptr %43, align 1, !dbg !28
  %45 = icmp ne i8 %44, 0, !dbg !28
  br i1 %45, label %46, label %52, !dbg !28, !prof !24

46:                                               ; preds = %38
  %47 = and i64 %40, 7, !dbg !28
  %48 = add i64 %47, 3, !dbg !28
  %49 = trunc i64 %48 to i8, !dbg !28
  %50 = icmp sge i8 %49, %44, !dbg !28
  br i1 %50, label %51, label %52, !dbg !28

51:                                               ; preds = %46
  call void @__asan_report_store4(i64 %40) #3, !dbg !28
  unreachable

52:                                               ; preds = %46, %38
  store i32 321, ptr %39, align 4, !dbg !28
  call void @_Z8sDeallocRPh(ptr %14), !dbg !29
  %53 = load i32, ptr %result, align 4, !dbg !29
  store i64 1172321806, ptr %15, align 8, !dbg !29
  %54 = icmp ne i64 %7, 0, !dbg !29
  br i1 %54, label %55, label %62, !dbg !29

55:                                               ; preds = %52
  %56 = add i64 %21, 0, !dbg !29
  %57 = inttoptr i64 %56 to ptr, !dbg !29
  store i64 -723401728380766731, ptr %57, align 1, !dbg !29
  %58 = add i64 %7, 56, !dbg !29
  %59 = inttoptr i64 %58 to ptr, !dbg !29
  %60 = load i64, ptr %59, align 8, !dbg !29
  %61 = inttoptr i64 %60 to ptr, !dbg !29
  store i8 0, ptr %61, align 1, !dbg !29
  br label %65, !dbg !29

62:                                               ; preds = %52
  %63 = add i64 %21, 0, !dbg !29
  %64 = inttoptr i64 %63 to ptr, !dbg !29
  store i64 0, ptr %64, align 1, !dbg !29
  br label %65, !dbg !29

65:                                               ; preds = %62, %55
  ret i32 %53, !dbg !29
}

declare ptr @_Z4sNewIiEPiv()

declare void @_Z8sDeallocRPh(ptr)

declare void @__asan_report_load_n(i64, i64)

declare void @__asan_loadN(i64, i64)

declare void @__asan_report_load1(i64)

declare void @__asan_load1(i64)

declare void @__asan_report_load2(i64)

declare void @__asan_load2(i64)

declare void @__asan_report_load4(i64)

declare void @__asan_load4(i64)

declare void @__asan_report_load8(i64)

declare void @__asan_load8(i64)

declare void @__asan_report_load16(i64)

declare void @__asan_load16(i64)

declare void @__asan_report_store_n(i64, i64)

declare void @__asan_storeN(i64, i64)

declare void @__asan_report_store1(i64)

declare void @__asan_store1(i64)

declare void @__asan_report_store2(i64)

declare void @__asan_store2(i64)

declare void @__asan_report_store4(i64)

declare void @__asan_store4(i64)

declare void @__asan_report_store8(i64)

declare void @__asan_store8(i64)

declare void @__asan_report_store16(i64)

declare void @__asan_store16(i64)

declare void @__asan_report_exp_load_n(i64, i64, i32)

declare void @__asan_exp_loadN(i64, i64, i32)

declare void @__asan_report_exp_load1(i64, i32)

declare void @__asan_exp_load1(i64, i32)

declare void @__asan_report_exp_load2(i64, i32)

declare void @__asan_exp_load2(i64, i32)

declare void @__asan_report_exp_load4(i64, i32)

declare void @__asan_exp_load4(i64, i32)

declare void @__asan_report_exp_load8(i64, i32)

declare void @__asan_exp_load8(i64, i32)

declare void @__asan_report_exp_load16(i64, i32)

declare void @__asan_exp_load16(i64, i32)

declare void @__asan_report_exp_store_n(i64, i64, i32)

declare void @__asan_exp_storeN(i64, i64, i32)

declare void @__asan_report_exp_store1(i64, i32)

declare void @__asan_exp_store1(i64, i32)

declare void @__asan_report_exp_store2(i64, i32)

declare void @__asan_exp_store2(i64, i32)

declare void @__asan_report_exp_store4(i64, i32)

declare void @__asan_exp_store4(i64, i32)

declare void @__asan_report_exp_store8(i64, i32)

declare void @__asan_exp_store8(i64, i32)

declare void @__asan_report_exp_store16(i64, i32)

declare void @__asan_exp_store16(i64, i32)

declare ptr @__asan_memmove(ptr, ptr, i64)

declare ptr @__asan_memcpy(ptr, ptr, i64)

declare ptr @__asan_memset(ptr, i32, i64)

declare void @__asan_handle_no_return()

declare void @__sanitizer_ptr_cmp(i64, i64)

declare void @__sanitizer_ptr_sub(i64, i64)

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i1 @llvm.amdgcn.is.shared(ptr) #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i1 @llvm.amdgcn.is.private(ptr) #1

declare i64 @__asan_stack_malloc_0(i64)

declare void @__asan_stack_free_0(i64, i64)

declare i64 @__asan_stack_malloc_1(i64)

declare void @__asan_stack_free_1(i64, i64)

declare i64 @__asan_stack_malloc_2(i64)

declare void @__asan_stack_free_2(i64, i64)

declare i64 @__asan_stack_malloc_3(i64)

declare void @__asan_stack_free_3(i64, i64)

declare i64 @__asan_stack_malloc_4(i64)

declare void @__asan_stack_free_4(i64, i64)

declare i64 @__asan_stack_malloc_5(i64)

declare void @__asan_stack_free_5(i64, i64)

declare i64 @__asan_stack_malloc_6(i64)

declare void @__asan_stack_free_6(i64, i64)

declare i64 @__asan_stack_malloc_7(i64)

declare void @__asan_stack_free_7(i64, i64)

declare i64 @__asan_stack_malloc_8(i64)

declare void @__asan_stack_free_8(i64, i64)

declare i64 @__asan_stack_malloc_9(i64)

declare void @__asan_stack_free_9(i64, i64)

declare i64 @__asan_stack_malloc_10(i64)

declare void @__asan_stack_free_10(i64, i64)

declare void @__asan_poison_stack_memory(i64, i64)

declare void @__asan_unpoison_stack_memory(i64, i64)

declare void @__asan_set_shadow_00(i64, i64)

declare void @__asan_set_shadow_01(i64, i64)

declare void @__asan_set_shadow_02(i64, i64)

declare void @__asan_set_shadow_03(i64, i64)

declare void @__asan_set_shadow_04(i64, i64)

declare void @__asan_set_shadow_05(i64, i64)

declare void @__asan_set_shadow_06(i64, i64)

declare void @__asan_set_shadow_07(i64, i64)

declare void @__asan_set_shadow_f1(i64, i64)

declare void @__asan_set_shadow_f2(i64, i64)

declare void @__asan_set_shadow_f3(i64, i64)

declare void @__asan_set_shadow_f5(i64, i64)

declare void @__asan_set_shadow_f8(i64, i64)

declare void @__asan_alloca_poison(i64, i64)

declare void @__asan_allocas_unpoison(i64, i64)

declare void @__asan_before_dynamic_init(i64)

declare void @__asan_after_dynamic_init()

declare void @__asan_register_globals(i64, i64)

declare void @__asan_unregister_globals(i64, i64)

declare void @__asan_register_image_globals(i64)

declare void @__asan_unregister_image_globals(i64)

declare void @__asan_register_elf_globals(i64, i64, i64)

declare void @__asan_unregister_elf_globals(i64, i64, i64)

declare void @__asan_init()

; Function Attrs: nounwind uwtable
define internal void @asan.module_ctor() #2 {
  call void @__asan_init()
  call void @__asan_version_mismatch_check_v8()
  ret void
}

declare void @__asan_version_mismatch_check_v8()

attributes #0 = { mustprogress noinline norecurse nounwind optnone sanitize_address uwtable }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #2 = { nounwind uwtable "frame-pointer"="all" }
attributes #3 = { nomerge }

!llvm.module.flags = !{!0, !1, !2, !3, !4, !5, !6}
!llvm.ident = !{!7}
!llvm.dbg.cu = !{!8}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{i32 7, !"Dwarf Version", i32 5}
!5 = !{i32 2, !"Debug Info Version", i32 3}
!6 = !{i32 4, !"nosanitize_address", i32 1}
!7 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!8 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !9, producer: "spice version dev (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None)

!10 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainv", scope: !11, file: !11, line: 1, type: !12, scopeLine: 1, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !8, retainedNodes: !15)

!12 = !DISubroutineType(types: !13)
!13 = !{!14}
!14 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!15 = !{}
!16 = !DILocalVariable(name: "result", scope: !10, file: !11, line: 1, type: !14)
!17 = !DILocation(line: 1, column: 1, scope: !10)
!18 = !DILocation(line: 2, column: 22, scope: !10)
!19 = !DILocalVariable(name: "iPtr", scope: !10, file: !11, line: 2, type: !20)
!20 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !14, size: 64)
!21 = !DILocation(line: 2, column: 5, scope: !10)
!22 = !DILocation(line: 3, column: 6, scope: !10)
!23 = !DILocation(line: 3, column: 13, scope: !10)
!24 = !{!"branch_weights", i32 1, i32 1048575}
!25 = !DILocation(line: 5, column: 35, scope: !26)
!26 = distinct !DILexicalBlock(scope: !10, file: !11, line: 4, column: 5)
!27 = !DILocation(line: 7, column: 6, scope: !10)
!28 = !DILocation(line: 7, column: 13, scope: !10)
!29 = !DILocation(line: 8, column: 1, scope: !10)
